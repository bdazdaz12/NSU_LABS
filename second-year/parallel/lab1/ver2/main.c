#include <stdio.h>
#include <mpi.h>
#include <malloc.h>
#include <math.h>

#define N 5011

int *sendVectorSize, *sendVectorStartPos, *sendMatrixSize, *sendMatrixStartPos;
int cntOfProcesses, rank;
const double epsilon = 0.000000001;

double *matrixMulVect(const double *matrixPart, const double *vectPart) {
    /*
     * передаем данные как цикличсекий сдвиг массива влево
     * считаем перевой итерацией передачи - загрузку наших начальных данных в буффер
     * умножаем на полученную часть, соответствующий ей "прямоугольник" в части матрицы
     * после вычислений передаем данные предыдущему процессу и получаем от следующего
     */
    double *result = (double*)calloc(sendVectorSize[rank], sizeof(double));
    double *receiveVectBuf = (double*)malloc(sendVectorSize[0] * sizeof(double)); //делаем буффер макс возм размера
    for (int i = 0; i < sendVectorSize[rank]; ++i) { // копируем "нулевую" принимаемую часть
        receiveVectBuf[i] = vectPart[i];
    }

    int ourSenderRank = (rank + 1) % cntOfProcesses; // индекс того, от кого процесс должен получить
    int ourRecipientRank = (rank + cntOfProcesses - 1) % cntOfProcesses;
    for (int ringIteration = 0, curSenderRank = rank; ringIteration < cntOfProcesses; ++ringIteration,
            curSenderRank = (curSenderRank + 1) % cntOfProcesses) {

        for (int i = 0; i < sendVectorSize[rank]; ++i) {  // индекс строки в matrixPart
            for (int j = 0; j < sendVectorSize[curSenderRank]; ++j) {
                result[i] += matrixPart[i * N + sendVectorStartPos[curSenderRank] + j] * receiveVectBuf[j];
            }
        }
        //TODO добавить условие для неотправки на последней итерации
        MPI_Sendrecv_replace(receiveVectBuf, sendVectorSize[0], MPI_DOUBLE, ourRecipientRank, 5,
                             ourSenderRank, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    return result;
}

void calcNextYn(double *aPart, double *xnPart, const double *bPart, double *ynPart) {
    /*
     * YN записываться с idx = 0
     */
    double *partA_xn = matrixMulVect(aPart, xnPart);
    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        ynPart[i] = partA_xn[i] - bPart[i];
        ///вычисляем соответств rank часть ynPart
    }
    free(partA_xn);
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

double calcNextTau(double *aPart, double *ynPart) {
    double *A_yn_part = matrixMulVect(aPart, ynPart);

    double numerator = scalarVectMul(ynPart, A_yn_part);
    double denominator = scalarVectMul(A_yn_part, A_yn_part);
    free(A_yn_part);
    return numerator / denominator;
}

void calcNextX(double *prevXnPart, double tau, const double *ynPart) {
    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        prevXnPart[i] -= tau * ynPart[i];
    }
}

double calcVectLen(double *v) {
    return sqrt(scalarVectMul(v, v));
}

double bLen;

int canFinish(double *aPart, double *xnPart, const double *bPart) {
    double *numerator = matrixMulVect(aPart, xnPart);

    for (int i = 0; i < sendVectorSize[rank]; ++i) {
        numerator[i] -= bPart[i];
    }
    int flag = (calcVectLen(numerator) / bLen) < epsilon;
    free(numerator);
    return flag;
}

void calcX(double *aPart, double *bPart, double *xnPart) {
    double *ynPart = (double*) malloc(sendVectorSize[rank] * sizeof(double));
    double tau;
    while (!canFinish(aPart, xnPart, bPart)) {
        calcNextYn(aPart, xnPart, bPart, ynPart);
        tau = calcNextTau(aPart, ynPart);
        calcNextX(xnPart, tau, ynPart);
    }
    free(ynPart);
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
        X[i] = 0.003123 * (i + 1);
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
        start = MPI_Wtime();
        loadData(A, B, X);
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
