#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

/* Декомпозируем входную область ОМЕГА в "линейную решетку"
 * Уровни будем разделять по направлению Z, сопоставляя каждому каждому процессу некоторый набор
 * "одномерных по Z" слоев, колво слоев на процессе равно Dx/cntOfProcs + psi(rank),
 * где psi(rank) - индикатор того, нужно ли давать дополнительный слой для оптимальной нагрузке на процессы */
/// облатсь декомпозируется СВЕРХУ ВНИЗУ по Z

#define Nx 16 // число узлов, на которые мы разбиваем сетку ОМЕГА по X
#define Ny 16 // число узлов сетки ОМЕГА по Y
#define Nz 16 // число узлов сетки ОМЕГА по Z, ДОЛЖНО БЫТЬ РАВНО ВЕРХНИМ
#define a 1
#define up 0
#define below 1

const double X0 = -1;
const double Y0 = -1;
const double Z0 = -1;
const double epsilon = 10e-8;
double iteration_constant;

double *curAndPrevPhi[2];
double *bufLayerFromOtherProc[2];

MPI_Request send_requests[2];
//объект для асинхронной отправки сообщений, для процесса СВЕРХУ и процесса СНИЗУ
MPI_Request recv_requests[2]; //аналогично для асинхронного приема сообщений
/* частичные значения функции */
double Fz, Fx, Fy;

int prevPhi = 0;
int curPhi = 1;

int rank, cntOfProcess;

double phi(double x, double y, double z) { // TODO done
    return x * x + y * y + z * z;
}

double rho(double x, double y, double z) { // TODO done
    return 6 - a * phi(x, y, z);
}

void calc_edges(int *flag, double hx, double hy, double hz, const int *layers, const int *offsets,
                double squared_hx, double squared_hy, double squared_hz) { // TODO done
    /// квадрат внутри квадрата
    /// Fi - это сеточная phi, которая, условно не зависит от точки пространства,
    /// а только от соседних значений на предыдущих итерациях
    int z;
    for (int x = 1; x < Nx - 1; ++x) {
        for (int y = 1; y < Ny - 1; ++y) {

            if (rank != 0) {
                z = 0;

                Fx = (curAndPrevPhi[prevPhi][z * Nx * Ny + (x + 1) * Ny + y] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + (x - 1) * Ny + y]) / squared_hx;

                Fy = (curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y + 1)] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y - 1)]) / squared_hy;

                Fz = (curAndPrevPhi[prevPhi][(z + 1) * Nx * Ny + x * Ny + y] +
                      bufLayerFromOtherProc[below][x * Ny + y]) / squared_hz;

                curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] =
                        (Fx + Fy + Fz - rho(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz))
                        / iteration_constant;

                if (fabs(curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] -
                         phi(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz)) > epsilon) {
                    *flag = 0;
                }
            }

            if (rank != cntOfProcess - 1) {
                z = layers[rank] - 1;

                Fx = (curAndPrevPhi[prevPhi][z * Nx * Ny + (x + 1) * Ny + y] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + (x - 1) * Ny + y]) / squared_hx;

                Fy = (curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y + 1)] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y - 1)]) / squared_hy;


                Fz = (curAndPrevPhi[prevPhi][(z - 1) * Nx * Ny + x * Ny + y] +
                      bufLayerFromOtherProc[up][x * Ny + y]) / squared_hz;

                curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] =
                        (Fx + Fy + Fz - rho(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz))
                        / iteration_constant;

                if (fabs(curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] -
                         phi(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz)) > epsilon) {
                    *flag = 0;
                }
            }
        }
    }
}

void fillLayers(const int *layers, const int *offsets, int Y, int Z, double Hx, double Hy, double Hz) {
    for (int i = 0, startLine = offsets[rank]; i <= layers[rank] - 1; i++, startLine++) {
        for (int j = 0; j <= Ny; j++) {
            for (int k = 0; k <= Nz; k++) {
                if ((startLine != 0) && (j != 0) && (k != 0) && (startLine != Nx) && (j != Ny) && (k != Nz)) {
                    curAndPrevPhi[0][i * Y * Z + j * Z + k] = 0;
                    curAndPrevPhi[1][i * Y * Z + j * Z + k] = 0;
                } else {
                    curAndPrevPhi[0][i * Y * Z + j * Z + k] = phi(startLine * Hx, j * Hy, k * Hz);
                    curAndPrevPhi[1][i * Y * Z + j * Z + k] = phi(startLine * Hx, j * Hy, k * Hz);
                }
            }
        }
    }
}

void send_data(int Y, int Z, const int *layers) {
    if (rank != 0) {
        /*передача сообщения без блокировки, обменялись верхними слоями*/
        MPI_Isend(&(curAndPrevPhi[prevPhi][0]), Z * Y, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD,
                  &send_requests[0]); //низ
        MPI_Irecv(bufLayerFromOtherProc[0], Z * Y, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &recv_requests[1]);
    }
    if (rank != cntOfProcess - 1) {
        /*передача сообщения без блокировки, обменялись нижними слоями*/
        MPI_Isend(&(curAndPrevPhi[prevPhi][(layers[rank] - 1) * Y * Z]), Z * Y, MPI_DOUBLE, rank + 1, 1,
                  MPI_COMM_WORLD, &send_requests[1]); //верх
        MPI_Irecv(bufLayerFromOtherProc[1], Z * Y, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &recv_requests[0]);
    }
}

void receive_data() { // ждем окончания обмена данными с соседними узлами-процессами
    if (rank != 0) {
        MPI_Wait(&recv_requests[1], MPI_STATUS_IGNORE);
        MPI_Wait(&send_requests[0], MPI_STATUS_IGNORE);
    }
    if (rank != cntOfProcess - 1) {
        MPI_Wait(&recv_requests[0], MPI_STATUS_IGNORE);
        MPI_Wait(&send_requests[1], MPI_STATUS_IGNORE);
    }
}

void calc_center(int *flag, int Y, int Z, double hx, double hy, double hz, const int *layers, const int *offsets,
                 double squared_hx, double squared_hy, double squared_hz) {
    for (int i = 1; i < layers[rank] - 1; ++i) {
        for (int j = 1; j < Ny; ++j) {
            for (int k = 1; k < Nz; ++k) {
                Fz = (curAndPrevPhi[prevPhi][(i + 1) * Y * Z + j * Z + k] +
                      curAndPrevPhi[prevPhi][(i - 1) * Y * Z + j * Z + k]) / squared_hx;
                Fx = (curAndPrevPhi[prevPhi][i * Y * Z + (j + 1) * Z + k] +
                      curAndPrevPhi[prevPhi][i * Y * Z + (j - 1) * Z + k]) / squared_hy;
                Fy = (curAndPrevPhi[prevPhi][i * Y * Z + j * Z + (k + 1)] +
                      curAndPrevPhi[prevPhi][i * Y * Z + j * Z + (k - 1)]) / squared_hz;
                curAndPrevPhi[curPhi][i * Y * Z + j * Z + k] =
                        (Fz + Fx + Fy - rho((i + offsets[rank]) * hx, j * hy, k * hz)) / iteration_constant;

                //условия выхода
                if (fabs(curAndPrevPhi[curPhi][i * Y * Z + j * Z + k] -
                         phi((i + offsets[rank]) * hx, j * hy, k * hz)) > epsilon) {
                    *flag = 0;
                }
            }
        }
    }
}

void find_max_diff(int Y, int Z, double hx, double hy, double hz, const int *layers, const int *offsets) {
    double max = 0;
    double F1;

    for (int i = 1; i < layers[rank] - 2; i++) {
        for (int j = 1; j < Ny; j++) {
            for (int k = 1; k < Nz; k++) {
                if ((F1 = fabs(curAndPrevPhi[curPhi][i * Y * Z + j * Z + k] -
                               phi((i + offsets[rank]) * hx, j * hy, k * hz))) > max) {
                    max = F1;
                }
            }
        }
    }
    double allProcMax = 0;
    MPI_Allreduce(&max, &allProcMax, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Max diff = %.9lf\n", allProcMax);
    }
}

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &cntOfProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        printf("Thread count: %d\n", cntOfProcess);
    }

    int *cntOfLayers = (int *) malloc(cntOfProcess * sizeof(int));
    int *offsets = (int *) malloc(cntOfProcess * sizeof(int));

    /*Распределение слоев по процессам*/
    for (int procRank = 0, height = Nz, maxRankWithAddLoad = height % cntOfProcess,
                 curLayer = 0; procRank < cntOfProcess; ++procRank) {
        offsets[procRank] = curLayer;
        cntOfLayers[procRank] = procRank < maxRankWithAddLoad ? (height / cntOfProcess + 1) : (height / cntOfProcess);
        curLayer += cntOfLayers[procRank];
    }

    int I = Nx; // размерность уровня "на процессе" по направлению X
    int J = Ny; // размерность уровня по направлению Y
    int K = cntOfLayers[rank]; // размерность уровня по направлению Z

    /* Выделение памяти для 3D пространства для предыдущей и текущей итерации */
    curAndPrevPhi[0] = (double *) malloc(I * J * K * sizeof(double));
    curAndPrevPhi[1] = (double *) malloc(I * J * K * sizeof(double));

    /* Вспомогательный буфер для хранения границ при обмене */
    bufLayerFromOtherProc[0] = (double *) malloc(K * J * sizeof(double));
    bufLayerFromOtherProc[1] = (double *) malloc(K * J * sizeof(double));

    /* Размеры области */
    double Dx = 2.0;
    double Dy = 2.0;
    double Dz = 2.0;

    /*Расстояния между соседними узлами сетки пространства по соответствующему координатному направлению */
    double hx = Dx / (Nx - 1);
    double hy = Dy / (Ny - 1);
    double hz = Dz / (Nz - 1);

    double squared_hx = pow(hx, 2);
    double squared_hy = pow(hy, 2);
    double squared_hz = pow(hz, 2);
    iteration_constant = 2 / squared_hx + 2 / squared_hy + 2 / squared_hz + a;
    //константа в начале формулы итерационного процесс метода Якоби

    /*Инициализация значений*/
    fillLayers(cntOfLayers, offsets, rank, J, K, hx, hy, hz);

    double start = MPI_Wtime();

    int atProcFlag, finalFlag;
    do {
        atProcFlag = 1;
        /*Обмениваемся краями*/
        send_data(J, K, rank, cntOfProcess, cntOfLayers);
        /*Считаем середину*/
        calc_center(&atProcFlag, J, K, hx, hy, hz, rank, cntOfLayers, offsets, squared_hx, squared_hy, squared_hz);
        /*Ждем получения всех данных*/
        receive_data(rank, cntOfProcess);
        /*Считаем края*/
        calc_edges(&atProcFlag, J, K, hx, hy, hz, rank, cntOfProcess, cntOfLayers, offsets, squared_hx, squared_hy,
                   squared_hz);
        swap(&prevPhi, &curPhi); // т.к. полученные на итерации моделирования, записываются на место предпредыдущего

        MPI_Allreduce(&atProcFlag, &finalFlag, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    } while (finalFlag == 0);

    double finish = MPI_Wtime();

    if (rank == 0) {
        printf("Time: %lf\n", finish - start);
    }
    find_max_diff(J, K, hx, hy, hz, rank, cntOfLayers, offsets);

    free(bufLayerFromOtherProc[0]);
    free(bufLayerFromOtherProc[1]);
    free(curAndPrevPhi[0]);
    free(curAndPrevPhi[1]);
    free(offsets);
    free(cntOfLayers);

    MPI_Finalize();
    return 0;
}

