#include <iostream>
#include <cmath>
#include <mpi.h>

#define N 5

int *sendVectorSize, *sendMatrixSize, *sendVectStartPos, *sendMatrixStartPos, *vectWorkZoneSize;
int *vectWorkZoneStartIdx;
int cntOfProcesses, rank;
const double epsilon = 0.00001;

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

void calcNextYn(double *aPart, double *xn, const double *bPart,
                double *ynPart, double *fullYN) {
    /*
     * YN записываться с idx = 0
     * но в НАЧАЛЬНЫХ данных он может "начинаться" не с 0
     */
    double *partA_xn = matrixMulVect(aPart, xn);
    for (int i = 0; i < vectWorkZoneSize[rank]; ++i) {
        ynPart[i] = partA_xn[i] - bPart[i];
        ///вычисляем соответств rank часть ynPart
    }
//    int a = 0;
//    for (int i = 0; i < cntOfProcesses; ++i){
//        printf("size = %d, startIdx = %d \n", vectWorkZoneSize[i], vectWorkZoneStartIdx[i]);
//    }
    MPI_Allgatherv(ynPart, vectWorkZoneSize[rank], MPI_DOUBLE,
                   fullYN, vectWorkZoneSize, vectWorkZoneStartIdx, MPI_DOUBLE, MPI_COMM_WORLD);
// TODO: эта сука выше ломает судьбы
    delete[](partA_xn);
}

double scalarVectMul(const double *v1, const double *v2) {
    /*
     * остальные считаю только произведение части координат, что хранятся у них
     * полное скалярное имеет только rank=0
     */
    double res, curNodeRes = 0;
    for (int i = 0; i < vectWorkZoneSize[rank]; ++i) {
        curNodeRes += v1[i] * v2[i];
    }
    MPI_Reduce(&curNodeRes, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank != 0){
        res = 1.0;
    }
    return res;
}

double calcNextTau(double *aPart, double *ynPart, double *fullYN) {
    double *A_yn_part = matrixMulVect(aPart, fullYN);
    double numerator = scalarVectMul(ynPart, A_yn_part);
    double denominator = scalarVectMul(A_yn_part, A_yn_part);
    delete[](A_yn_part);
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
    MPI_Bcast(&flag, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
    return flag;
}

void calcX(double *aPart, double *bPart, double *xnFull) {
    auto *ynPart = new double [vectWorkZoneSize[rank]];
    auto *fullYN = new double [N];
    double tau = 0;
    int cnt = 0;
    while (!canFinish(aPart, xnFull, bPart) && cnt < 50000) {
        calcNextYn(aPart, xnFull, bPart, ynPart, fullYN);
        tau = calcNextTau(aPart, ynPart, fullYN);
        MPI_Bcast(&tau, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); ///этого не правил
        calcNextX(xnFull, tau, fullYN);
        cnt++;
    }
    delete[](ynPart); /// здесь ошибки каво?
    delete[](fullYN);
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
        vectWorkZoneStartIdx[i] = offsetIdx / N;
        offsetIdx += sendMatrixSize[i];
        vectWorkZoneSize[i] = sendMatrixSize[i] / N;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0){
        for (int i = 0; i < cntOfProcesses; ++i){
            std::cout << sendMatrixSize[i] << " " << sendMatrixStartPos[i] << "\n";
        }
        std::cout << "\n";
        std::cout << N % cntOfProcesses << " \n";
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
            std::cout << xPart[i] << " ";
        }
        std::cout << "\n";
        delete[](xPart);
    }
    MPI_Finalize();
    return 0;
}
