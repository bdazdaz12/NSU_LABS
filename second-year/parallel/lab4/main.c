#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

/* Декомпозируем входную область ОМЕГА в "линейную решетку"
 * Уровни будем разделять по направлению Z, сопоставляя каждому каждому процессу некоторый набор
 * "одномерных по Z" слоев, колво слоев на процессе равно Dx/cntOfProcs + psi(rank),
 * где psi(rank) - индикатор того, нужно ли давать дополнительный слой для оптимальной нагрузке на процессы */
/// облатсь декомпозируется СВЕРХУ ВНИЗУ по Z

#define Nx 60 // число узлов, на которые мы разбиваем сетку ОМЕГА по X
#define Ny 60 // число узлов сетки ОМЕГА по Y
#define Nz 60 // число узлов сетки ОМЕГА по Z, ДОЛЖНО БЫТЬ РАВНО ВЕРХНИМ
#define a 1
#define up 0
#define below 1

const double X0 = -1;
const double Y0 = -1;
const double Z0 = -1;
const double epsilon = 10e-8;
double iteration_constant;

double *curAndPrevPhi[2];
double *bufferedLvlFromOtherProc[2];

MPI_Request send_requests[2];
//объект для асинхронной отправки сообщений, для процесса СВЕРХУ и процесса СНИЗУ
MPI_Request recv_requests[2]; //аналогично для асинхронного приема сообщений
/* частичные значения функции */
double Fz, Fx, Fy;

int prevPhi = 0, curPhi = 1;

int rank, cntOfProcess;

double phi(double x, double y, double z) { // TODO done
    return x * x + y * y + z * z;
}

double rho(double x, double y, double z) { // TODO done
    return 6 - a * phi(x, y, z);
}

//для следующей итерации считаем новые значения для 2х слоев - верхнего и нижнего (в зависимости от контекста процесса)
void calc_edges(int *flag, const double hx, const double hy, const double hz, const int *levels, const int *offsets,
                const double squared_hx, const double squared_hy, const double squared_hz) { // TODO done
    /// "КВАДРАТ ВНУТРИ КВАДРАТА"

    /// Fi - это сеточная phi, которая, условно не зависит от точки пространства,
    /// а только от соседних значений на предыдущих итерациях

    // граничные точки(грани) по X и Y - неизменны, потому что известены точные для искомой функции для каждого процесса
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
                      bufferedLvlFromOtherProc[below][x * Ny + y]) / squared_hz;

                curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] =
                        (Fx + Fy + Fz - rho(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz))
                        / iteration_constant;

                if (fabs(curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] -
                curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + y]) > epsilon) {
                    *flag = 0;
                }
            }

            if (rank != cntOfProcess - 1) {
                z = levels[rank] - 1;

                Fx = (curAndPrevPhi[prevPhi][z * Nx * Ny + (x + 1) * Ny + y] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + (x - 1) * Ny + y]) / squared_hx;

                Fy = (curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y + 1)] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y - 1)]) / squared_hy;


                Fz = (curAndPrevPhi[prevPhi][(z - 1) * Nx * Ny + x * Ny + y] +
                      bufferedLvlFromOtherProc[up][x * Ny + y]) / squared_hz;

                curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] =
                        (Fx + Fy + Fz - rho(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz))
                        / iteration_constant;

                if (fabs(curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] -
                curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + y]) > epsilon) {
                    *flag = 0;
                }
            }
        }
    }
}

void fillLevels(const int *levels, const int *offsets, const double hx, const double hy, const double hz) {
    for (int z = 0, curLevel = offsets[rank]; z < levels[rank]; ++z, ++curLevel) {
        for (int x = 0; x < Nx; ++x) {
            for (int y = 0; y < Ny; ++y) {
                if ((curLevel != 0) && (x != 0) && (y != 0) && (curLevel != Nz) && (x != Nx - 1) && (y != Ny - 1)) {
                    curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + y] = 0;
                    curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] = 0;
                } else {
                    curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + y] =
                            phi(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz);
                    curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] =
                            phi(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz);
                            /// это в принципе для curPhi не нужно
                }
            }
        }
    }
}

void send_data(const int *levels) {
    if (rank != 0) {
        // отправляем нижний слой на процесс "вниз"
        MPI_Isend(&(curAndPrevPhi[prevPhi][0]), Nx * Ny, MPI_DOUBLE, rank - 1,
                  0, MPI_COMM_WORLD,&send_requests[0]);
        // принимаем слой от процесса "снизу", т.е. его верхний слой
        MPI_Irecv(bufferedLvlFromOtherProc[0], Nx * Ny, MPI_DOUBLE, rank - 1,
                  1, MPI_COMM_WORLD, &recv_requests[1]);
    }
    if (rank != cntOfProcess - 1) {
        /* обменялись верхними слоями*/
        MPI_Isend(&(curAndPrevPhi[prevPhi][(levels[rank] - 1) * Nx * Ny]), Nx * Ny, MPI_DOUBLE, rank + 1,
                  1, MPI_COMM_WORLD, &send_requests[1]);
        MPI_Irecv(bufferedLvlFromOtherProc[1], Nx * Ny, MPI_DOUBLE, rank + 1,
                  0, MPI_COMM_WORLD, &recv_requests[0]);
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

void calc_center(int *flag, const double hx, const double hy, const double hz, const int *levels, const int *offsets,
                 const double squared_hx, const double squared_hy, const double squared_hz) {
    for (int z = 1; z < levels[rank] - 1; ++z) {
        for (int x = 1; x < Nx - 1; ++x) {
            for (int y = 1; y < Ny - 1; ++y) {
                Fx = (curAndPrevPhi[prevPhi][z * Nx * Ny + (x + 1) * Ny + y] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + (x - 1) * Ny + y]) / squared_hx;

                Fy = (curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y + 1)] +
                      curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + (y - 1)]) / squared_hy;

                Fz = (curAndPrevPhi[prevPhi][(z + 1) * Nx * Ny + x * Ny + y] +
                        curAndPrevPhi[prevPhi][(z - 1) * Nx * Ny + x * Ny + y]) / squared_hz;

                curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] =
                        (Fx + Fy + Fz - rho(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz))
                        / iteration_constant;

                //условие того, что функция пока недостаточно близка в некотором узле == 0
                if (fabs(curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y]-
                curAndPrevPhi[prevPhi][z * Nx * Ny + x * Ny + y]) > epsilon) {
                    *flag = 0;
                }
            }
        }
    }
}

void find_max_diff(const double hx, const double hy, const double hz, const int *levels, const int *offsets) {
    double max = 0;
    double F;

    for (int z = 1; z < levels[rank] - 1; ++z) {
        for (int x = 0; x < Nx; ++x) {
            for (int y = 0; y < Ny; ++y) {
                if ((F = fabs(curAndPrevPhi[curPhi][z * Nx * Ny + x * Ny + y] -
                              phi(X0 + x * hx, Y0 + y * hy, Z0 + (offsets[rank] + z) * hz))) > max) {
                    max = F;
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

    int *levels = (int *) malloc(cntOfProcess * sizeof(int)); // колво уровней сопоставляемых процессу
    int *offsets = (int *) malloc(cntOfProcess * sizeof(int));

    /*Распределение слоев по процессам*/
    for (int procRank = 0, height = Nz, maxRankWithAddLoad = height % cntOfProcess,
                 curLevel = 0; procRank < cntOfProcess; ++procRank) {
        offsets[procRank] = curLevel;
        levels[procRank] = procRank < maxRankWithAddLoad ? (height / cntOfProcess + 1) : (height / cntOfProcess);
        curLevel += levels[procRank];
    }

    /* Выделение памяти для 3D пространства для предыдущей и текущей итерации */
    /// Содержит в памяти "модель" только своего уровня
    curAndPrevPhi[prevPhi] = (double *) malloc(Nx * Ny * levels[rank] * sizeof(double));
    curAndPrevPhi[curPhi] = (double *) malloc(Nx * Ny * levels[rank] * sizeof(double));

    /* Вспомогательный буфер для хранения границ при обмене */
    bufferedLvlFromOtherProc[0] = (double *) malloc(Nx * Ny * sizeof(double));
    bufferedLvlFromOtherProc[1] = (double *) malloc(Nx * Ny * sizeof(double));

    /* Размеры области */
    const double Dx = 2.0;
    const double Dy = 2.0;
    const double Dz = 2.0;

    /*Расстояния между соседними узлами сетки пространства по соответствующему координатному направлению */
    const double hx = Dx / (Nx - 1);
    const double hy = Dy / (Ny - 1);
    const double hz = Dz / (Nz - 1);

    const double squared_hx = pow(hx, 2);
    const double squared_hy = pow(hy, 2);
    const double squared_hz = pow(hz, 2);
    iteration_constant = 2 / squared_hx + 2 / squared_hy + 2 / squared_hz + a;
    //константа в начале формулы итерационного процесс метода Якоби

    /*Инициализация значений*/
    fillLevels(levels, offsets, hx, hy, hz);

    double start = MPI_Wtime();

    int atProcFlag, finalFlag;
    do {
        atProcFlag = 1;
        /*Обмениваемся краями*/
        send_data(levels);
        /*Считаем середину*/
        calc_center(&atProcFlag, hx, hy, hz, levels, offsets, squared_hx, squared_hy, squared_hz);
        /*Ждем получения всех данных*/
        receive_data();
        /*Считаем края*/
        calc_edges(&atProcFlag, hx, hy, hz, levels, offsets, squared_hx, squared_hy, squared_hz);
        swap(&prevPhi, &curPhi); // т.к. полученные на итерации моделирования, записываются на место предпредыдущего

        MPI_Allreduce(&atProcFlag, &finalFlag, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        atProcFlag = finalFlag;
    } while (atProcFlag == 0);

    double finish = MPI_Wtime();

    if (rank == 0) {
        printf("Time: %lf\n", finish - start);
    }
    find_max_diff(hx, hy, hz, levels, offsets);

    free(bufferedLvlFromOtherProc[0]);
    free(bufferedLvlFromOtherProc[1]);
    free(curAndPrevPhi[0]);
    free(curAndPrevPhi[1]);
    free(offsets);
    free(levels);

    MPI_Finalize();
    return 0;
}

