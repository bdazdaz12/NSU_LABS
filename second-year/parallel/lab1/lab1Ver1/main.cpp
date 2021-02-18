#include <iostream>
#include <cmath>
#include <mpi.h>

#define N 11

int *sendVectorSize, *sendMatrixSize, *sendVectStartPos, *sendMatrixStartPos, *vectWorkZoneSize;
int *vectWorkZoneStartIdx;
int cntOfProcesses, rank;
const double epsilon = 0.00005;

double *matrixMulVect(const double *matrixPart, const double *vectPart);

double *calcNextYn(double *aPart, double *xnPart, const double *bPart);

double scalarVectMul(const double *v1, const double *v2);

double calcNextTau(double *aPart, double *ynPart);

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

    double *aPart, *bPart, *xPart;
    //// в процессе rank==0 это все полные начальные данные(A, b, x), в других узлах - только их часть

    calcSendDataParam();
    allocMem(&aPart, &bPart, &xPart);
    if (rank == 0) {
        loadData(aPart, bPart, xPart);
    }

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
    delete[](vectWorkZoneSize);
    delete[](vectWorkZoneStartIdx);
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
    /*
     * записывает результат в буфер начиная с idx=0 до idx = колво строк в отправ матрице
     * матрица и сам вектор считают своим началом idx = 0
     * но в самих изначальных данных они являються частью чего-то большего и имеют там свои индексы
     */
    auto *res = (double *) calloc(vectWorkZoneSize[rank], sizeof(double));
    for (int i = 0; i < vectWorkZoneSize[rank]; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i] += matrixPart[i * N + j] * vectPart[j];
        }
    }
    return res;
}

double *calcNextYn(double *aPart, double *xnPart, const double *bPart) {
    /*
     * YN записываться с idx = 0
     * но в НАЧАЛЬНЫХ данных он может "начинаться" не с 0
     */
    auto *ynPart = (double *) calloc(N , sizeof(double)); // поставил N вместо рабочей части, потому что нужен целиком
    double *partA_xn = matrixMulVect(aPart, xnPart);
    for (int i = 0; i < vectWorkZoneSize[rank]; ++i) {
        ynPart[i] = partA_xn[i] - bPart[i];
        ///вычисляем соответств rank часть ynPart
    }
    //TODO объединить YN после подсчета частей в процессах;
//    MPI_Allgatherv(ynPart, vectWorkZoneSize[rank], MPI_DOUBLE,
//                   ynPart, vectWorkZoneSize, vectWorkZoneStartIdx, MPI_DOUBLE, MPI_COMM_WORLD);
    delete[](partA_xn);
    return ynPart;
}

double scalarVectMul(const double *v1, const double *v2) {
    /*
     * реальное скалярное произведение имеет только rank=0
     * остальные считаю только произведение части координат, что хранятся у них
     */
    double res = 0, curNodeRes = 0;
    for (int i = 0; i < vectWorkZoneSize[rank]; ++i) {
        curNodeRes += v1[i] * v2[i];
    }
    MPI_Reduce(&curNodeRes, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        res = 1; ///костыльчик
    }
    return res;
}

double calcNextTau(double *aPart, double *ynPart) {
    double *A_yn = matrixMulVect(aPart, ynPart);
    double numerator = scalarVectMul(ynPart, A_yn);
    double denominator = scalarVectMul(A_yn, A_yn);
    delete[](A_yn);
    return numerator / denominator;
}

double *calcNextX(double *prevXPart, double tau, const double *ynPart) {
    for (int i = 0; i < vectWorkZoneSize[rank]; ++i) {
        prevXPart[i] -= tau * ynPart[i];
    }
    //TODO мы имеем только часть след XN, но каждому процессу нужна полная часть. ОБЪЕДИНИТЬ НИЖЕ после этого
//    MPI_Allgatherv(prevXPart, vectWorkZoneSize[rank], MPI_DOUBLE,
//                   prevXPart, vectWorkZoneSize, vectWorkZoneStartIdx, MPI_DOUBLE, MPI_COMM_WORLD);
    return prevXPart;
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
    bool flag = true;
    double *numerator = matrixMulVect(aPart, xnPart);
    for (int i = 0; i < vectWorkZoneSize[rank]; ++i) {
        numerator[i] -= bPart[i];
    }
    flag = (calcVectLen(numerator) / bLen) < epsilon;
    delete[](numerator);
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    return flag;
}

inline void calcX(double *aPart, double *bPart, double *xPart) {
    double *ynPart, tau;
    while (!canFinish(aPart, xPart, bPart)) {
        printf("cycle, tay = %f \n", tau);
        ynPart = calcNextYn(aPart, xPart, bPart);
        tau = calcNextTau(aPart, ynPart);
        MPI_Bcast(&tau, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        xPart = calcNextX(xPart, tau, ynPart);
    }
//    MPI_Gatherv(xPart, vectWorkZoneSize[rank], MPI_DOUBLE,
//                xPart, vectWorkZoneSize, vectWorkZoneStartIdx, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

inline void calcSendDataParam() {
    sendVectorSize = new int[cntOfProcesses];
    sendVectStartPos = new int[cntOfProcesses];
    sendMatrixSize = new int[cntOfProcesses];
    sendMatrixStartPos = new int[cntOfProcesses];
    vectWorkZoneSize = new int[cntOfProcesses];
    vectWorkZoneStartIdx = new int[cntOfProcesses];
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
        vectWorkZoneSize[i] = sendMatrixSize[i] / N;
        vectWorkZoneStartIdx[i] = offsetIdx / N;
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
