#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <omp.h>

#define N 1020

const double epsilon = 0.0000456;

double *matrixMulVect(const double *matrix, const double *vector) {
    double *res = (double *) calloc(N, sizeof(double));
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i] += matrix[i * N + j] * vector[j];
        }
    }
    return res;
}

void calcNextYn(double *A, double *xn, const double *B, double *yn) {
    double *A_xn = matrixMulVect(A, xn);
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        yn[i] = A_xn[i] - B[i];
    }
    free(A_xn);
}

double scalarVectMul(const double *v1, const double *v2) {
    double scalarMulRes = 0;
#pragma omp parallel for //TODO добавить редус
    for (int i = 0; i < N; ++i) {
        scalarMulRes += v1[i] * v2[i]; //TODO вот здесь может коллизия обращения?
    }
    return scalarMulRes;
}

double calcNextTau(double *A, double *yn) {
    double *A_yn = matrixMulVect(A, yn);
    double numerator = scalarVectMul(yn, A_yn);
    double denominator = scalarVectMul(A_yn, A_yn);
    free(A_yn);
    return numerator / denominator;
}

void calcNextX(double *prevX, double tau, const double *yn) {
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        prevX[i] -= tau * yn[i];
    }
}

double calcVectLen(double *v) {
    return sqrt(scalarVectMul(v, v));
}

double bLen;

int canFinish(double *A, double *xn, const double *B) {
    double *numerator = matrixMulVect(A, xn);
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        numerator[i] -= B[i];
    }
    int flag = (calcVectLen(numerator) / bLen) < epsilon;
    free(numerator);
    return flag;
}

void calcX(double *A, double *B, double *xn) {
    double *yn = (double *) malloc(N * sizeof(double));
    double tau;
    while (!canFinish(A, xn, B)) {
        calcNextYn(A, xn, B, yn);
        tau = calcNextTau(A, yn);
        calcNextX(xn, tau, yn);
    }
    free(yn);
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
    for (int i = 0; i < 13; ++i) {
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

    bLen = calcVectLen(B);
    calcX(A, B, X);

    free(A);
    free(B);
    printf("ans\n");
    for (int i = 0; i < 13; ++i) {
        printf("%f ", X[i]);
    }
    printf("\n");
    free(X);
}