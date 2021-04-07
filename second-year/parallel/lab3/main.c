#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

#define n1 16
#define n2 16
#define n3 16 // сделать кратным 16, для сбора C

#define ndims 2 // размерность сетки
#define X 0 // номер измерения X. Отвечает за направление "по строкам".
#define Y 1 // направление "по столбцам". число - "индекс доступа".

#define debug

int gridRank, rowRank, columnRank, cntOfProcesses;
int *sendRowCnt, *sendRowBeginPos, *sendRowSize;
int *sendColumnCnt, *sendColumnBeginPos, *sendColumnSize;

void createCommunicators(MPI_Comm *gridComm, MPI_Comm *rowComm, MPI_Comm *columnComm,
                         int *dims, int *coords) {
    int reorder = 1; // флаг о проведении перенумеровки процессов для топологии сетки
    int periods[ndims] = {0, 0}; // логический массив с флагами цикличности по каждому из направлений

    MPI_Dims_create(cntOfProcesses, ndims, dims); // определение оптимальной кофигурации сетки и запись ее в dims
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, gridComm); // создание коммуникатора сетки
    MPI_Comm_rank(*gridComm, &gridRank);
    MPI_Cart_get(*gridComm, 2, dims, periods, coords); // получение координат процесса в сетке

    MPI_Comm_split(*gridComm, coords[Y], coords[X], rowComm); // Здесь "color" == нахождение в одной строке
    MPI_Comm_split(*gridComm, coords[X], coords[Y], columnComm);
    /*
     *  split -  разделение топологии сетки на непересекающиеся подгруппы. Разделение по признаку "color".
     * "key" - для упорядочивания процессов внутри подгруппы.
     * "одинаковые" группы - получатся с "одинаковой с 0" нумерацией
     */
    rowRank = coords[X];
    columnRank = coords[Y];
}

void calcSendDataParams(const int *dims) {
    sendRowCnt = (int *) malloc(sizeof(int) * dims[X]);
    sendRowBeginPos = (int *) malloc(sizeof(int) * dims[X]);
    sendRowSize = (int *) malloc(sizeof(int) * dims[X]);

    sendColumnCnt = (int *) malloc(sizeof(int) * dims[Y]);
    sendColumnBeginPos = (int *) malloc(sizeof(int) * dims[Y]);
    sendColumnSize = (int *) malloc(sizeof(int) * dims[Y]);

    int offsetIdx = 0;
    for (int rankInRowComm = 0; rankInRowComm < dims[X]; ++rankInRowComm) {
        if (rankInRowComm < n1 % dims[X]) {
            sendRowCnt[rankInRowComm] = n1 / dims[X] + 1;
        } else {
            sendRowCnt[rankInRowComm] = n1 / dims[X];
        }
        sendRowSize[rankInRowComm] = sendRowCnt[rankInRowComm] * n2;
        sendRowBeginPos[rankInRowComm] = offsetIdx;
        offsetIdx += sendRowSize[rankInRowComm];
    }

    offsetIdx = 0;
    for (int rankInColumnComm = 0; rankInColumnComm < dims[Y]; ++rankInColumnComm) {
        if (rankInColumnComm < n3 % dims[Y]) {
            sendColumnCnt[rankInColumnComm] = n3 / dims[Y] + 1;
        } else {
            sendColumnCnt[rankInColumnComm] = n3 / dims[Y];
        }
        sendColumnSize[rankInColumnComm] = sendColumnCnt[rankInColumnComm] * n2;
        sendColumnBeginPos[rankInColumnComm] = offsetIdx;
        offsetIdx += sendColumnCnt[rankInColumnComm];
    }
}

void fillData(double **A, double **B, double **C) {
    *A = (double *) malloc(sizeof(double) * n1 * n2);
    *B = (double *) malloc(sizeof(double) * n2 * n3);
    *C = (double *) malloc(sizeof(double) * n1 * n3);

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < n2; ++j) {
            (*A)[i * n2 + j] = i;
        }
    }
    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < n3; ++j) {
            (*B)[i * n3 + j] = i == j ? 1 : 0;
        }
    }
}

void sendMatricesParts(double *A, double *B, double *A_part, double *B_part,
                       MPI_Comm rowComm, MPI_Comm columnComm, int *coords) {
    if (coords[Y] == 0) { // рассылаем A по первому столбцу по всем его строкам
        MPI_Scatterv(A, sendRowSize, sendRowBeginPos, MPI_DOUBLE,
                     A_part, sendRowSize[rowRank], MPI_DOUBLE, 0, rowComm);
    }
    if (coords[X] == 0) { // рассылаем B по первой строке и всем её столбцам
        MPI_Datatype row_t;
        MPI_Type_contiguous(n2, MPI_DOUBLE, &row_t);
        MPI_Type_commit(&row_t);

        MPI_Datatype column_t;
        MPI_Type_vector(n2, 1, n3, MPI_DOUBLE, &column_t);
        MPI_Type_commit(&column_t);

        MPI_Datatype columnShell_t;
        MPI_Type_create_resized(column_t, 0, sizeof(double), &columnShell_t);
        MPI_Type_commit(&columnShell_t);

        MPI_Scatterv(B, sendColumnCnt, sendColumnBeginPos, columnShell_t,
                     B_part, sendColumnCnt[columnRank], row_t, 0, columnComm);

        MPI_Type_free(&column_t);
        MPI_Type_free(&row_t);
        MPI_Type_free(&columnShell_t);
    }
    MPI_Bcast(A_part, sendRowSize[rowRank], MPI_DOUBLE, 0, columnComm);
    MPI_Bcast(B_part, sendColumnSize[columnRank], MPI_DOUBLE, 0, rowComm);
}

double *calcMatricesMul(const double *A_part, const double *B_part) {
    double *C_part = (double *) calloc(sendRowCnt[rowRank] * sendColumnCnt[columnRank], sizeof(double));

    for (int i = 0; i < sendRowCnt[rowRank]; ++i) { // индексы строки в A_part и столбца в B_part
        for (int k = 0; k < sendColumnCnt[columnRank]; ++k) { // индекс движения по столбцам
            for (int j = 0; j < n2; ++j) { // индекс движения по строке и столбцу
                C_part[i * sendColumnCnt[columnRank] + k] += A_part[i * n2 + j] * B_part[k * n2 + j];
            } // записываем здесь по полной строке в C_part
        }
    }
    return C_part;
}

void collecting_C(double *C_part, double *C, const int *dims, MPI_Comm gridComm) { // TODO оно не работает
    // TODO: пока что это работает только с матрицами у которых sendColumnCnt одинаковый для всех частей
    // gridRank = coords[X] * dims[Y] + coords[Y]

    int *displs = (int*) malloc(sizeof(int) * cntOfProcesses);
    int *recvCounts = (int*) malloc(sizeof(int) * cntOfProcesses);

    int offset = 0;
    for (int i = 0; i < dims[X]; ++i) {
        for (int j = 0; j < dims[Y]; ++j) {
            displs[i * dims[Y] + j] = offset;
            recvCounts[i * dims[Y] + j] = sendRowCnt[i];
            offset += sendColumnCnt[j];
        }
        offset += (sendRowCnt[i] - 1) * n3;
    }

    MPI_Datatype row_t;
    MPI_Type_contiguous(sendColumnCnt[0], MPI_DOUBLE, &row_t);
    MPI_Type_commit(&row_t);

    MPI_Datatype rowShell_t;
    MPI_Type_create_resized(row_t, 0, n3 * sizeof(double), &rowShell_t);
    MPI_Type_commit(&rowShell_t);

    MPI_Gatherv(C_part, sendRowCnt[rowRank], row_t, C, recvCounts, displs, rowShell_t, 0, gridComm);

    MPI_Type_free(&row_t);
    MPI_Type_free(&rowShell_t);

    free(displs);
    free(recvCounts);
}

void print_C_parts(double *C_part) {
    for (int i = 0; i < cntOfProcesses; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (gridRank == i) {
            printf("gridRank =  %d\n", gridRank);
            for (int j = 0; j < sendRowCnt[rowRank]; ++j) {
                for (int k = 0; k < sendColumnCnt[columnRank]; ++k) {
                    printf("%f ", C_part[j * sendColumnCnt[columnRank] + k]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
}

void printMatrix(double *matrix, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%f ", matrix[i * m + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &cntOfProcesses);

    int dims[ndims] = {0, 0}; // числов процессов вдоль каждого измерения (X, Y)
    int coords[ndims]; // координаты процесса в 2d сетке

    MPI_Comm gridComm;
    MPI_Comm rowComm;
    MPI_Comm columnComm;

    createCommunicators(&gridComm, &rowComm, &columnComm, dims, coords);
    calcSendDataParams(dims);

    double *A = NULL, *B = NULL, *C = NULL;
    double *A_part = (double *) malloc(sizeof(double) * sendRowCnt[rowRank] * n2);
    double *B_part = (double *) malloc(sizeof(double) * sendColumnCnt[columnRank] * n2);
    if (gridRank == 0) {
        fillData(&A, &B, &C);

#ifdef debug
        printf("matrix A:\n");
        printMatrix(A, n1, n2);
        printf("matrix B:\n");
        printMatrix(B, n2, n3);
#endif
    }

    sendMatricesParts(A, B, A_part, B_part, rowComm, columnComm, coords);

    double *C_part = calcMatricesMul(A_part, B_part);

#ifdef debug
    print_C_parts(C_part);
#endif

//    collecting_C(C_part, C, dims, gridComm); //TODO тут валиться

    free(A_part);
    free(B_part);
    free(C_part);

    free(sendRowSize);
    free(sendRowBeginPos);
    free(sendRowCnt);
    free(sendColumnSize);
    free(sendColumnBeginPos);
    free(sendColumnCnt);

    if (gridRank == 0) {
//        printf("matrix C:\n");
//        printMatrix(C, n1, n3);

        free(A);
        free(B);
        free(C);
    }
    MPI_Finalize();
}
