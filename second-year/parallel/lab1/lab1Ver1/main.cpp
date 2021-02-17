#include <iostream>
#include <cmath>
#include <mpi.h>

#define N 11

int *sendVectorSize, *sendMatrixSize, *sendVectStartPos, *sendMatrixStartPos;
int cntOfProcesses, rank;
const double epsilon = 0.00005;

double *matrixMulVect(const double *matrixPart, const double *vectPart);

double *calcNextYn(double *aPart, double *xnPart, const double *bPart);

double scalarVectMul(const double *v1, const double *v2);

double calcNextTau(double *aPart, double *yn);

double *calcNextX(double *prevXPart, double tau, const double *ynPart);

double calcVectLen(double *v);

bool canFinish(double *aPart, double *xnPart, double *bPart);

inline void calcX(double *aPart, double *bPart, double *xPart);
/*
 * раскидать недостающие строки по одной в каждый процесс ранга rank < N % cnt
 * ну и загрузку данных проводить придется так же
 */
inline void calcSendDataParam();
inline void allocMem(double **aPart, double **bPart, double **xPart);
inline void loadData(double *matrixPart, double *bPart, double *xPart);

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cntOfProcesses);

    // эти данные имеют(вычисляют) все, чтобы не считать для всех все постоянно
    double *aPart, *bPart, *xPart;
    //// в процессе rank==0 это все полные начальные данные(A, b, x), в других узлах - только их часть

    calcSendDataParam();
    allocMem(&aPart, &bPart, &xPart);
    if (rank == 0) {
        loadData(aPart, bPart, xPart);
    }

    //TODO: раздать всем данные и начать считать
    MPI_Scatterv(aPart, sendMatrixSize, sendMatrixStartPos, MPI_DOUBLE,
                 aPart, sendMatrixSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем A из rank=0
    MPI_Scatterv(bPart, sendVectorSize, sendVectStartPos, MPI_DOUBLE,
                 bPart, sendVectorSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем b из rank=0
    MPI_Scatterv(xPart, sendVectorSize, sendVectStartPos, MPI_DOUBLE,
                 xPart, sendVectorSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем x из rank=0

    calcX(aPart, bPart, xPart);

    delete[](aPart);
    delete[](bPart);
    delete[](sendVectorSize);////
    delete[](sendMatrixSize);
    delete[](sendVectStartPos);
    delete[](sendMatrixStartPos);
    if (rank != 0) {
        delete[](xPart);
    } else {
        printf("\n");
        for (int i = 0; i < N; ++i){
            std::cout << xPart[i] << "\n";
        }
        delete[](xPart);
    }
    MPI_Finalize();
}


double *matrixMulVect(const double *matrixPart, const double *vectPart) {
    auto *res = (double *) calloc(sendVectorSize[rank], sizeof(double));
    for (int i = 0; i < sendMatrixSize[rank] / N; ++i) { //МАТРИЦУ отправляем строкой кратной N, но идти нужно по столб
        for (int j = 0; j < N; ++j) {
            res[i] += matrixPart[i * N + j] * vectPart[j]; //результат по idx=0 начинает записываться
        }
    }
    return res;
}

double *calcNextYn(double *aPart, double *xnPart, const double *bPart) { //TODO
    auto *yn = (double *) calloc(N, sizeof(double));
    double *firstArg = matrixMulVect(aPart, xnPart);
    for (int i = 0; i < sendMatrixSize[rank] / N; ++i) {
        yn[sendMatrixStartPos[rank] / N + i] = firstArg[i] - bPart[sendMatrixStartPos[rank] / N + i];
        ///вычисляем соответств rank часть yn
    }
    delete[](firstArg);
    return yn;
}

double scalarVectMul(const double *v1, const double *v2) {
    double res = 0, curNodeRes = 0;
    for (int i = sendVectStartPos[rank]; i < sendVectorSize[rank]; ++i) {
        curNodeRes += v1[i] * v2[i];
    } //TODO: проверить правильность
    MPI_Reduce(&curNodeRes, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        res = 1; ///костыльчик
    }
    return res;
}

double calcNextTau(double *aPart, double *yn) { //TODO собрать тау
    double *A_yn = matrixMulVect(aPart, yn);
    double numerator = scalarVectMul(yn, A_yn);
    double denominator = scalarVectMul(A_yn, A_yn);
    delete[](A_yn);
    return numerator / denominator;
}

double *calcNextX(double *prevXPart, double tau, const double *ynPart) {
    for (int i = 0; i < N; ++i) {
        prevXPart[i] -= tau * ynPart[i];
    }
    return prevXPart; ///TODO собрать X
}

double calcVectLen(double *v) {
    return sqrt(scalarVectMul(v, v));
}

bool canFinish(double *aPart, double *xnPart, double *bPart) {
    static double bLen = calcVectLen(bPart);
    bool flag = true;
    if (rank == 0) {
        double *numerator = matrixMulVect(aPart, xnPart);
        for (int i = 0; i < N; ++i) {
            numerator[i] -= bPart[i];
        }
        flag = (calcVectLen(numerator) / bLen) < epsilon;
        delete[](numerator);
    }
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    return flag;
}

inline void calcX(double *aPart, double *bPart, double *xPart) {
    double *yn, tau;
    while (!canFinish(aPart, xPart, bPart)) {
        yn = calcNextYn(aPart, xPart, bPart);
        tau = calcNextTau(aPart, yn);
        xPart = calcNextX(xPart, tau, yn);
    }
}

inline void calcSendDataParam() {
    sendVectorSize = new int[cntOfProcesses];
    sendVectStartPos = new int[cntOfProcesses];
    sendMatrixSize = new int[cntOfProcesses];
    sendMatrixStartPos = new int[cntOfProcesses];
    int offsetIdx = 0;
    for (int i = 0; i < cntOfProcesses; ++i) {
        sendVectorSize[i] = N; /// N / cntOfProcesses + 1 при разрезании
        sendVectStartPos[i] = 0;
        if (i < N % cntOfProcesses) {
            sendMatrixSize[i] = N * N / cntOfProcesses + N;
        } else {
            sendMatrixSize[i] = N * N / cntOfProcesses;
        }
        sendMatrixStartPos[i] = offsetIdx;
        offsetIdx += sendMatrixSize[i];
    }
}

inline void allocMem(double **aPart, double **bPart, double **xPart) {
    if (rank == 0) {
        *aPart = new double[N * N];
    } else {
        *aPart = new double[sendMatrixSize[rank]];
    }
    *bPart = new double[N]; ////
    *xPart = new double[N];
}

inline void loadData(double *matrixPart, double *bPart, double *xPart) {
    for (int i = 0; i < N; ++i) {
        bPart[i] = N + 1;
        xPart[i] = 0;
        for (int j = 0; j < N; ++j) {
            matrixPart[i * N + j] = i == j ? 2.0 : 1.0;
        }
    }
}
