#include <iostream>
#include <cmath>
#include <mpi.h>

#define N 201

int *sendMatrixSize, *sendMatrixStartPos, *aPartCntOfStrs, *aPartBeginPosInFull;
int cntOfProcesses, rank;
const double epsilon = 0.00001;

double *matrixMulVect(const double *matrixPart, const double *vector) {
    auto *res = (double *) calloc(aPartCntOfStrs[rank], sizeof(double));
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
        ynPart[i] = partA_xn[i] - B[i];
        ///вычисляем соответств rank часть ynPart
    }
    MPI_Allgatherv(ynPart, aPartCntOfStrs[rank], MPI_DOUBLE,
                   fullYN, aPartCntOfStrs, aPartBeginPosInFull, MPI_DOUBLE, MPI_COMM_WORLD);
    delete[](partA_xn);
}

double scalarVectMul(const double *v1, const double *v2) {
    /*
     * остальные считаю только произведение части координат, что хранятся у них
     * полное скалярное имеет только rank=0
     */
    double res, curNodeRes = 0;
    for (int i = 0; i < aPartCntOfStrs[rank]; ++i) {
        curNodeRes += v1[i] * v2[i];
    }
    MPI_Reduce(&curNodeRes, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); //TODO вынести отсюда
    if (rank != 0) {     ///использовать это сразу после вызова scalar отправив 0ому рез ото всех
        res = 1.0; ////костыльчик
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

bool canFinish(double *aPart, double *xn, double *B) {
    /*
     * numerator: каждый процесс независимо может посчитать его, соотв процессу, часть
     * собирать его не нужно т.к. нам нужно будет посчитать его скалярное произведение,
     * а оно автоматически собираеться в rank=0
     */
    static double bLen = calcVectLen(B);
    double *numerator = matrixMulVect(aPart, xn);
    for (int i = 0; i < aPartCntOfStrs[rank]; ++i) {
        numerator[i] -= B[i];
    }
    bool flag = (calcVectLen(numerator) / bLen) < epsilon; /// TODO переделать
//    bool flag = true;
//    if (rank == 0) {
//        flag = (calcVectLen(numerator) / bLen) < epsilon;
//    }
    MPI_Bcast(&flag, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
    delete[](numerator);
    return flag;
}

void calcX(double *aPart, double *B, double *xn) {
    auto *ynPart = new double[aPartCntOfStrs[rank]];
    auto *fullYN = new double[N];
    double tau;
    while (!canFinish(aPart, xn, B)) {
        calcNextYn(aPart, xn, B, ynPart, fullYN);
        tau = calcNextTau(aPart, ynPart, fullYN);
        MPI_Bcast(&tau, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        calcNextX(xn, tau, fullYN);
    }
    delete[](ynPart);
    delete[](fullYN);
}

inline void calcSendDataParam() {
    sendMatrixSize = new int[cntOfProcesses];
    sendMatrixStartPos = new int[cntOfProcesses];
    aPartCntOfStrs = new int[cntOfProcesses];
    aPartBeginPosInFull = new int[cntOfProcesses];
    int offsetIdx = 0;
    for (int procRank = 0; procRank < cntOfProcesses; ++procRank) {
        if (procRank < N % cntOfProcesses) {
            sendMatrixSize[procRank] = (N / cntOfProcesses + 1) * N; //целочисленное деление не оч
        } else {
            sendMatrixSize[procRank] = (N / cntOfProcesses) * N;
        }
        sendMatrixStartPos[procRank] = offsetIdx;
        aPartBeginPosInFull[procRank] = offsetIdx / N;
        offsetIdx += sendMatrixSize[procRank];
        aPartCntOfStrs[procRank] = sendMatrixSize[procRank] / N;
    }
}

inline void allocMem(double **aPart, double **B, double **X) {
    *aPart = new double[sendMatrixSize[rank]];
    *B = new double[N];
    *X = new double[N];
}

inline void loadData(double *A, double *B, double *X) {
    for (int i = 0; i < N; ++i) {
        B[i] = N + 1;
        X[i] = 0;
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = i == j ? 2.0 : 1.0;
        }
    }
}


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cntOfProcesses);

    double *aPart, *B, *X;
    auto *A = new double[N * N];

    calcSendDataParam();
    allocMem(&aPart, &B, &X);
    if (rank == 0) {
        loadData(A, B, X);
    }

    MPI_Scatterv(A, sendMatrixSize, sendMatrixStartPos, MPI_DOUBLE,
                 aPart, sendMatrixSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); ///отсылаем A из rank=0
    MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(X, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    calcX(aPart, B, X);

    delete[](aPart);
    delete[](A);
    delete[](B);
    delete[](sendMatrixSize);
    delete[](sendMatrixStartPos);
    delete[](aPartCntOfStrs);
    delete[](aPartBeginPosInFull);
    if (rank == 0) {
        printf("answer:\n");
        for (int i = 0; i < 13; ++i) { ///пусть будет не N печатать долго
            std::cout << X[i] << " ";
        }
        std::cout << "\n";
    }
    delete[](X);
    MPI_Finalize();
}
