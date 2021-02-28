#include <stdio.h>
#include <mpi.h>
#include <malloc.h>
#include <math.h>

#define N 13

int *sendVectorSize, *sendVectorStartPos, *sendMatrixSize, *sendMatrixStartPos;
int cntOfProcesses, rank;
const double epsilon = 0.0001;

double *matrixMulVect(const double *matrixPart, const double *vectPart) {
    /*
     * matrixPart - представляет собой несколько подряд идущих транспонированных столбцов исходной матрицы
     * после вычисления полученные "составляющие" итогового веткора - объединяються в rank=0
     * после, сответствующие части итогового результата рассылаються в соответствующие процессы
     */
    double *result = (double *) calloc(N, sizeof(double));
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
    double *full_A_xn = (double *) calloc(N, sizeof(double));
    MPI_Allreduce(partA_xn, full_A_xn, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        ynPart[i] = full_A_xn[sendVectorStartPos[rank] + i] - bPart[i];
        ///вычисляем соответств rank часть ynPart
    }
    MPI_Allgatherv(ynPart, sendVectorSize[rank], MPI_DOUBLE,
                   fullYN, sendVectorSize, sendVectorStartPos, MPI_DOUBLE, MPI_COMM_WORLD);
    free(partA_xn);
    free(full_A_xn);
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
    double *full_A_yn = (double *) calloc(N, sizeof(double));
    MPI_Allreduce(A_yn_part, full_A_yn, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    double numerator = scalarVectMul(ynPart, full_A_yn + sendVectorStartPos[rank]);
    double denominator = scalarVectMul(full_A_yn + sendVectorStartPos[rank],
                                       full_A_yn + sendVectorStartPos[rank]); // адресная арифметика
    free(A_yn_part);
    free(full_A_yn);
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

double bLen;

int canFinish(double *aPart, double *xnPart, const double *bPart) {
    /*
     * numerator: каждый процесс независимо может посчитать его, соотв процессу, часть
     * собирать его не нужно т.к. нам нужно будет посчитать его скалярное произведение,
     * а оно автоматически собираеться в rank=0
     */
    double *numerator = matrixMulVect(aPart, xnPart);
    double *fullNumerator = (double *) calloc(N, sizeof(double));
    MPI_Allreduce(numerator, fullNumerator, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        fullNumerator[sendVectorStartPos[rank] + i] -= bPart[i];
    }
    int flag = (calcVectLen(fullNumerator + sendVectorStartPos[rank]) / bLen) < epsilon;
    free(numerator);
    free(fullNumerator);
    return flag;
}

void calcX(double *aPart, double *bPart, double *xnFull) {
    double *ynPart = (double*) malloc(sendVectorSize[rank] * sizeof(double));
    double *fullYN = (double*) malloc(N * sizeof(double));
    double tau;
    while (!canFinish(aPart, xnFull, bPart)) {
        calcNextYn(aPart, xnFull, bPart, ynPart, fullYN);
        tau = calcNextTau(aPart, ynPart, fullYN);
        calcNextX(xnFull, tau, fullYN);
    }
    free(ynPart);
    free(fullYN);
}

void calcSendDataParam() {
    sendVectorSize = (int*) malloc(cntOfProcesses * sizeof(int));
    sendVectorStartPos = (int*) malloc(cntOfProcesses * sizeof(int));
    sendMatrixSize = (int*) malloc(cntOfProcesses * sizeof(int));
    sendMatrixStartPos = (int*) malloc(cntOfProcesses * sizeof(int));
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

void allocMem(double **aPart, double **bPart, double **xPart) {
    *aPart = (double*) malloc(sendMatrixSize[rank] * sizeof(double));
    *bPart = (double*) malloc(sendMatrixSize[rank] * sizeof(double));
    *xPart = (double*) malloc(sendMatrixSize[rank] * sizeof(double));
}

void loadData(double *A, double *B, double *X) {
    for (int i = 0; i < N; ++i) {
        X[i] = 0.003123 * i;
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = i == j ? 0.00131 : 0.0009;
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i] += A[i * N + j] * X[j];
        }
    }
    if (rank == 0) {
        for (int i = 0; i < 13; ++i){
            printf("%f ", X[i]);
        }
        printf("\n\n");
    }
    for (int i = 0; i < N; ++i) {
        X[i] = 0;
    }
}

void transposeMatrix(double *A) {
    double tmp;
    for (int i = 1; i < N; ++i) {
        for (int j = 0; j < i; ++j) {
            tmp = A[i * N + j];
            A[i * N + j] = A[j * N + i];
            A[j * N + i] = tmp;
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cntOfProcesses);

    double *aPart, *bPart, *xPart;
    double *A = (double*) malloc(N * N * sizeof(double));
    double *B = (double*) calloc(N, sizeof(double));
    double *X = (double*) malloc(N * sizeof(double));


    calcSendDataParam();
    allocMem(&aPart, &bPart, &xPart);
    double start;
    if (rank == 0) {
        loadData(A, B, X);
        start = MPI_Wtime();
        transposeMatrix(A);
    }

    MPI_Scatterv(A, sendMatrixSize, sendMatrixStartPos, MPI_DOUBLE,
                 aPart, sendMatrixSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем A из rank=0
    MPI_Scatterv(B, sendVectorSize, sendVectorStartPos, MPI_DOUBLE,
                 bPart, sendVectorSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем B из rank=0
    MPI_Scatterv(X, sendVectorSize, sendVectorStartPos, MPI_DOUBLE,
                 xPart, sendVectorSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем X из rank=0

    bLen = calcVectLen(bPart);
    calcX(aPart, bPart, xPart);
    MPI_Gatherv(xPart, sendVectorSize[rank], MPI_DOUBLE,
                X, sendVectorSize, sendVectorStartPos, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(aPart);
    free(bPart);
    free(xPart);
    free(sendVectorSize);
    free(sendMatrixSize);
    free(sendVectorStartPos);
    free(sendMatrixStartPos);
    free(A);
    free(B);
    if (rank == 0) {
        double end = MPI_Wtime();
        FILE *outFile = fopen("time.txt", "w");
        fprintf(outFile, "time = %f\n", end - start);
        fclose(outFile);
        printf("ans\n");
        for (int i = 0; i < 13; ++i){
            printf("%f ", X[i]);
        }
        printf("\n");
    }
    free(X);
    MPI_Finalize();
}
