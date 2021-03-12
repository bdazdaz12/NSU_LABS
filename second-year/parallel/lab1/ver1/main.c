#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <malloc.h>
#include <math.h>

#define N 255

int *sendMatrixSize, *sendMatrixStartPos, *aPartCntOfStrs, *strBeginPosInFull;
int cntOfProcesses, rank;
const double epsilon = 0.0000456;

double *matrixMulVect(const double *matrixPart, const double *vector) {
    double *res = (double *) calloc(aPartCntOfStrs[rank], sizeof(double));
    for (int i = 0; i < aPartCntOfStrs[rank]; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i] += matrixPart[i * N + j] * vector[j];
        }
    }
    return res;
}

void calcNextYn(double *aPart, double *xn, const double *B, double *ynPart, double *fullYN) {
    /*
     * YN записываться с idx = 0
     * но в НАЧАЛЬНЫХ данных он может "начинаться" не с 0
     */
    double *partA_xn = matrixMulVect(aPart, xn);
    for (int i = 0; i < aPartCntOfStrs[rank]; ++i) {
        ynPart[i] = partA_xn[i] - B[strBeginPosInFull[rank] + i];
        ///вычисляем соответств rank часть ynPart
    }
    MPI_Allgatherv(ynPart, aPartCntOfStrs[rank], MPI_DOUBLE,
                   fullYN, aPartCntOfStrs, strBeginPosInFull, MPI_DOUBLE, MPI_COMM_WORLD);
    free(partA_xn);
}

double scalarVectMul(const double *v1, const double *v2) { //TODO
    /*
     * в конце метода все процессы собирают с друг друга части полного итогового произведения
     */
    double curNodeRes = 0, scalarMulRes = 0;
    for (int i = 0; i < aPartCntOfStrs[rank]; ++i) {
        curNodeRes += v1[i] * v2[i];
    }
    MPI_Allreduce(&curNodeRes, &scalarMulRes, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return scalarMulRes;
}

double calcNextTau(double *aPart, double *ynPart, double *fullYN) {
    double *A_yn_part = matrixMulVect(aPart, fullYN);
    double numerator = scalarVectMul(ynPart, A_yn_part);
    double denominator = scalarVectMul(A_yn_part, A_yn_part);
    free(A_yn_part);
    return numerator / denominator;
}

void calcNextX(double *prevX, double tau, const double *fullYN) {
    for (int i = 0; i < N; ++i) {
        prevX[i] -= tau * fullYN[i];
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

int canFinish(double *aPart, double *xn, const double *B) {
    /*
     * numerator: каждый процесс независимо может посчитать его, соотв процессу, часть
     * собирать его не нужно т.к. нам нужно будет посчитать его скалярное произведение,
     * а оно автоматически собираеться в rank=0
     */
    double *numerator = matrixMulVect(aPart, xn);
    for (int i = 0; i < aPartCntOfStrs[rank]; ++i) {
        numerator[i] -= B[strBeginPosInFull[rank] + i]; //TODO проеб с этим вектором B
    }

    double curNorma = (calcVectLen(numerator) / bLen);
   // if (rank == 0) {
       // printf("cur flag = %f \n", curNorma);
   // }
    int flag = curNorma < epsilon;

    //    int flag = (calcVectLen(numerator) / bLen) < epsilon;
    free(numerator);
    return flag;
}

void calcX(double *aPart, double *B, double *xn) {
    double *ynPart = (double *)malloc(aPartCntOfStrs[rank] * sizeof(double));
    double *fullYN = (double *)malloc(N * sizeof(double ));
    double tau;
    while (!canFinish(aPart, xn, B)) {
        calcNextYn(aPart, xn, B, ynPart, fullYN);
        tau = calcNextTau(aPart, ynPart, fullYN);
        calcNextX(xn, tau, fullYN);
    }
    free(ynPart);
    free(fullYN);
}

void calcSendDataParam() {
    sendMatrixSize = (int*)malloc(cntOfProcesses * sizeof(int));
    sendMatrixStartPos = (int*)malloc(cntOfProcesses * sizeof(int));
    aPartCntOfStrs = (int*)malloc(cntOfProcesses * sizeof(int));
    strBeginPosInFull = (int*)malloc(cntOfProcesses * sizeof(int));
    int offsetIdx = 0;
    for (int procRank = 0; procRank < cntOfProcesses; ++procRank) {
        if (procRank < N % cntOfProcesses) {
            sendMatrixSize[procRank] = (N / cntOfProcesses + 1) * N; //целочисленное деление не оч
        } else {
            sendMatrixSize[procRank] = (N / cntOfProcesses) * N;
        }
        sendMatrixStartPos[procRank] = offsetIdx;
        strBeginPosInFull[procRank] = offsetIdx / N;
        offsetIdx += sendMatrixSize[procRank];
        aPartCntOfStrs[procRank] = sendMatrixSize[procRank] / N;
    }
}

void allocMem(double **aPart, double **B, double **X) {
    *aPart = (double*)malloc(sendMatrixSize[rank] * sizeof(double));;
    *B = (double*)calloc(N, sizeof(double));
    *X = (double*)malloc(N * sizeof(double));
}

void loadData(double *A, double *B, double *X) {
    srand(1);
    for (int i = 0; i < N; ++i) {
        X[i] = (double)(rand() % 18 - 9);
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = i == j ? (double)(rand() % 18 - 9) : 0.0009;
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i] += A[i * N + j] * X[j];
        }
    }
    if (rank == 0) {
        for (int i = N - 10; i < N; ++i){
            printf("%f ", X[i]);
        }
        printf("\n\n");
    }
    for (int i = 0; i < N; ++i) {
        X[i] = 0;
    }
}

int main(int argc, char **argv) {
	srand(1);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cntOfProcesses);

    double *aPart, *B, *X;
    double *A = (double*) malloc(N * N * sizeof(double));
    calcSendDataParam();
    allocMem(&aPart, &B, &X);
    double start = 0;
    if (rank == 0) {
        loadData(A, B, X);
        start = MPI_Wtime();
    }

    MPI_Scatterv(A, sendMatrixSize, sendMatrixStartPos, MPI_DOUBLE,
                 aPart, sendMatrixSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем A из rank=0
    MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(X, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    bLen = calcVectLen(B + strBeginPosInFull[rank]);
    calcX(aPart, B, X);

    free(aPart);
    free(A);
    free(B);
    free(sendMatrixSize);
    free(sendMatrixStartPos);
    free(aPartCntOfStrs);
    free(strBeginPosInFull);
    if (rank == 0) {
        double end = MPI_Wtime();
        FILE *outFile = fopen("time.txt", "a");
        printf("%d\n", N);
        fprintf(outFile, "N = %d, cntOfProcesses = %d, time = %f\n", N, cntOfProcesses, end - start);
        fclose(outFile);
        printf("ans\n");
        for (int i = N - 10; i < N; ++i){
            printf("%f ", X[i]);
        }
        printf("\n");
    }
    free(X);
    MPI_Finalize();
}
