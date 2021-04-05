#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <omp.h>

#define N 401

const double epsilon = 0.000456;


double *matrixMulVectRes, *YN;

double *matrixMulVect(const double *matrix, const double *vector) {
#pragma omp single
    {
        matrixMulVectRes = (double *) calloc(N, sizeof(double));
    }
#pragma omp for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrixMulVectRes[i] += matrix[i * N + j] * vector[j];
        }
    }
    return matrixMulVectRes;
}

void calcNextYn(double *A, double *xn, const double *B) {
    double *A_xn = matrixMulVect(A, xn);
#pragma omp for
    for (int i = 0; i < N; ++i) {
        YN[i] = A_xn[i] - B[i];
    }
#pragma omp single
    {
        free(A_xn);
    }
}

double scalarMulRes = 0;
double scalarMulResBuf;

double scalarVectMul(const double *v1, const double *v2) { // TODO добавить shared
#pragma omp for reduction(+:scalarMulRes)
    for (int i = 0; i < N; ++i) {
        scalarMulRes += v1[i] * v2[i];
    }
#pragma omp single
    {
        scalarMulResBuf = scalarMulRes;
        scalarMulRes = 0;
    }
    return scalarMulResBuf;
}

double calcNextTau(double *A) {
    double *A_yn = matrixMulVect(A, YN);
    double numerator = scalarVectMul(YN, A_yn);
    double denominator = scalarVectMul(A_yn, A_yn);
#pragma omp single
    {
        free(A_yn);
    }
    return numerator / denominator;
}

void calcNextX(double *prevX, double tau) {
#pragma omp for
    for (int i = 0; i < N; ++i) {
        prevX[i] -= tau * YN[i];
    }
}

double calcVectLen(double *v) {
    return sqrt(scalarVectMul(v, v));
}

double bLen;

int canFinish(double *A, double *xn, const double *B) {
    double *numerator = matrixMulVect(A, xn);
#pragma omp for
    for (int i = 0; i < N; ++i) {
        numerator[i] -= B[i];
    }
    int flag = (calcVectLen(numerator) / bLen) < epsilon;
#pragma omp single
    {
        free(numerator);
    }
    return flag;
}

void calcX(double *A, double *B, double *xn) {
    bLen = calcVectLen(B);
#pragma omp single
    {
        YN = (double *) malloc(N * sizeof(double));
    }
    double tau;
    while (!canFinish(A, xn, B)) {
        calcNextYn(A, xn, B);
        tau = calcNextTau(A);
        calcNextX(xn, tau);
    }
#pragma omp single
    {
        free(YN);
    }
}

void loadData(double *A, double *B, double *X) {
    srand(1);
    for (int i = 0; i < N; ++i) {
        X[i] = (double) (rand() % 18 - 9);
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = i == j ? (double) (rand() % 18 - 9) : 0.0009;
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i] += A[i * N + j] * X[j];
        }
    }
    for (int i = N - 10; i < N; ++i) {
        printf("%f ", X[i]);
    }
    printf("\n\n");
    for (int i = 0; i < N; ++i) {
        X[i] = 0;
    }
}


int main(int argc, char **argv) {
    double *A = (double *) malloc(N * N * sizeof(double));
    double *B = (double *) calloc(N, sizeof(double));
    double *X = (double *) malloc(N * sizeof(double));

    loadData(A, B, X);

#pragma omp parallel
    {
        calcX(A, B, X);
    }

    free(A);
    free(B);
    printf("ans\n");
    for (int i = N - 10; i < N; ++i) {
        printf("%f ", X[i]);
    }
    printf("\n");
    free(X);
}
