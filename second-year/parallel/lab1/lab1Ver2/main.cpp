#include <iostream>
#include <cmath>
#include <mpi.h>

#define N 13

int *sendVectorSize, *sendVectorStartPos, *sendMatrixSize, *sendMatrixStartPos;
int cntOfProcesses, rank;
const double epsilon = 0.00001;

double *matrixMulVect(const double *matrixPart, const double *vectPart) {
    /*
     * matrixPart - представляет собой несколько подряд идущих транспонированных столбцов исходной матрицы
     * после вычисления полученные "составляющие" итогового веткора - объединяються в rank=0
     * после, сответствующие части итогового результата рассылаються в соответствующие процессы
     */
    auto *result = (double *) calloc(N, sizeof(double));
    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        for (int j = 0; j < N; ++j) {
            result[j] += matrixPart[i * N + j] * vectPart[i];
        }
    }
    return result;
}

void calcNextYn(double *aPart, double *xn, const double *bPart, double *ynPart, double *fullYN) {
    /*
     * YN записываться с idx = 0
     * но в НАЧАЛЬНЫХ данных он может "начинаться" не с 0
     */
    double *partA_xn = matrixMulVect(aPart, xn);
    auto *full_A_xn = (double *) calloc(N, sizeof(double));
    MPI_Allreduce(partA_xn, full_A_xn, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        ynPart[i] = full_A_xn[sendVectorStartPos[rank] + i] - bPart[i];
        ///вычисляем соответств rank часть ynPart
    }
    MPI_Allgatherv(ynPart, sendVectorSize[rank], MPI_DOUBLE,
                   fullYN, sendVectorSize, sendVectorStartPos, MPI_DOUBLE, MPI_COMM_WORLD);
    delete[](partA_xn);
    delete[](full_A_xn);
}

double scalarVectMul(const double *v1, const double *v2) {
    /*
     * в конце метода все процессы собирают с друг друга части полного итогового произведения
     */
    double curNodeRes = 0, scalarMulRes = 0;
    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        curNodeRes += v1[i] * v2[i];
    }
    MPI_Allreduce(&curNodeRes, &scalarMulRes, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return scalarMulRes;
}

double calcNextTau(double *aPart, double *ynPart, double *fullYN) {
    double *A_yn_part = matrixMulVect(aPart, fullYN);
    auto *full_A_yn = (double *) calloc(N, sizeof(double));
    MPI_Allreduce(A_yn_part, full_A_yn, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    double numerator = scalarVectMul(ynPart, full_A_yn + sendVectorStartPos[rank]);
    double denominator = scalarVectMul(full_A_yn + sendVectorStartPos[rank],
                                       full_A_yn + sendVectorStartPos[rank]); // адресная арифметика
    delete[](A_yn_part);
    delete[](full_A_yn);
    return numerator / denominator;
}

void calcNextX(double *prevX, double tau, const double *fullYN) {
    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        prevX[i] -= tau * fullYN[sendVectorStartPos[rank] + i];
    }
    /*
     * т.к. мы имеем на пред итерации полный XN, а YN нам придеться собрать на текущей итерации
     * то мы имеем на каждом узле все данные для вычисления этого полностью до N
     */
}

double calcVectLen(double *v) {
    return sqrt(scalarVectMul(v, v));
}

bool canFinish(double *aPart, double *xnPart, double *bPart) {
    /*
     * numerator: каждый процесс независимо может посчитать его, соотв процессу, часть
     * собирать его не нужно т.к. нам нужно будет посчитать его скалярное произведение,
     * а оно автоматически собираеться в rank=0
     */
    static double bLen = calcVectLen(bPart);
    double *numerator = matrixMulVect(aPart, xnPart);
    auto *fullNumerator = (double *) calloc(N, sizeof(double));
    MPI_Allreduce(numerator, fullNumerator, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        fullNumerator[sendVectorStartPos[rank] + i] -= bPart[i];
    }
    bool flag = (calcVectLen(fullNumerator + sendVectorStartPos[rank]) / bLen) < epsilon;
    delete[](numerator);
    delete[](fullNumerator);
    return flag;
}

void calcX(double *aPart, double *bPart, double *xnFull) {
    auto *ynPart = new double[sendVectorSize[rank]];
    auto *fullYN = new double[N];
    double tau;
    while (!canFinish(aPart, xnFull, bPart)) {
        calcNextYn(aPart, xnFull, bPart, ynPart, fullYN);
        tau = calcNextTau(aPart, ynPart, fullYN);
        calcNextX(xnFull, tau, fullYN);
    }
    delete[](ynPart);
    delete[](fullYN);
}

inline void calcSendDataParam() {
    sendVectorSize = new int[cntOfProcesses];
    sendVectorStartPos = new int[cntOfProcesses];
    sendMatrixSize = new int[cntOfProcesses];
    sendMatrixStartPos = new int[cntOfProcesses];
    int offsetIdx = 0;
    for (int procRank = 0; procRank < cntOfProcesses; ++procRank) {
        if (procRank < N % cntOfProcesses) {
            sendMatrixSize[procRank] = (N / cntOfProcesses + 1) * N; //целочисленное деление не оч
        } else {
            sendMatrixSize[procRank] = (N / cntOfProcesses) * N;
        }
        sendMatrixStartPos[procRank] = offsetIdx;
        sendVectorStartPos[procRank] = offsetIdx / N;
        sendVectorSize[procRank] = sendMatrixSize[procRank] / N;
        offsetIdx += sendMatrixSize[procRank];
    }
}

inline void allocMem(double **aPart, double **bPart, double **xPart) {
    *aPart = new double[sendMatrixSize[rank]];
    *bPart = new double[sendVectorSize[rank]];
    *xPart = new double[sendVectorSize[rank]];
}

void loadData(double *A, double *B, double *X) {
    for (int i = 0; i < N; ++i) {
        B[i] = N + 1;
        X[i] = 0;
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = i == j ? 2.0 : 1.0;
        }
    }
}

inline void transposeMatrix(double *A) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::swap(A[i * N + j], A[j * N + i]);
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cntOfProcesses);

    double *aPart, *bPart, *xPart;
    auto *A = new double[N * N];
    auto *B = new double[N];
    auto *X = new double[N];


    calcSendDataParam();
    allocMem(&aPart, &bPart, &xPart);
    if (rank == 0) {
        loadData(A, B, X);
        transposeMatrix(A);
    }

    MPI_Scatterv(A, sendMatrixSize, sendMatrixStartPos, MPI_DOUBLE,
                 aPart, sendMatrixSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем A из rank=0
    MPI_Scatterv(B, sendVectorSize, sendVectorStartPos, MPI_DOUBLE,
                 bPart, sendVectorSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем B из rank=0
    MPI_Scatterv(X, sendVectorSize, sendVectorStartPos, MPI_DOUBLE,
                 xPart, sendVectorSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем X из rank=0

    calcX(aPart, bPart, xPart);
    MPI_Gatherv(xPart, sendVectorSize[rank], MPI_DOUBLE,
                X, sendVectorSize, sendVectorStartPos, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    delete[](aPart);
    delete[](bPart);
    delete[](xPart);
    delete[](sendVectorSize);
    delete[](sendMatrixSize);
    delete[](sendVectorStartPos);
    delete[](sendMatrixStartPos);
    delete[](A);
    delete[](B);
    if (rank == 0) {
        printf("answer\n");
        for (int i = 0; i < 13; ++i) {
            std::cout << X[i] << " ";
        }
        std::cout << "\n";
    }
    delete[](X);
    MPI_Finalize();
}
