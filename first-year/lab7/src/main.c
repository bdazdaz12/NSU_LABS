#include<stdio.h>
#include<stdlib.h>

int readGraph(char* adjMatrix, int n, int m){
    for (int i = 0 ; i < m; ++i){
        int from, to;
        if(scanf("%d%d", &from, &to) != 2){
            printf("bad number of lines");
            return 0;
        }
        if (from < 1 || from > n || to < 1 || to > n){
            printf("bad vertex");
            return 0;
        }
        adjMatrix[(from - 1) * n + (to - 1)] = 1;
    }
    return 1;
}

char topDFS(char* adjMatrix, char* color, short vertex, short* freeInStack, short* ansStack, short n){
    if(color[vertex] == 1){
        printf("impossible to sort");
        return 0;
    } else if (color[vertex] == 2){
        return 1;
    }
    color[vertex]++;
    for (short i = 0; i < n; ++i){
        if (adjMatrix[vertex * n + i]){
            if (!topDFS(adjMatrix, color, i, freeInStack, ansStack, n)){
                return 0;
            }
        }
    }
    color[vertex]++;
    ansStack[(*freeInStack)++] = vertex + 1;
    return 1;
}

int main(){
    int m, n;
    char err = 0;
    if(scanf("%d%d", &n, &m) != 2) {
        printf("bad number of lines");
        return 0;
    }
    if (n < 0 || n > 1000){
        printf("bad number of vertices");
        return 0;
    }
    if (m < 0 || m > n * (n + 1) >> 1){
        printf("bad number of edges");
        return 0;
    }
    short* ansStack = (short*)malloc(2 * n);
    short freeInStack = 0;
    char* color = (char*)malloc(n);
    char* adjMatrix = (char*)malloc(n * n);
    for (int i = 0; i < n; ++i){
        color[i] = 0;
        for (int j = 0; j < n; ++j){
            adjMatrix[i * n + j] = 0;
        }
    }
    if (!readGraph(adjMatrix, n, m))///tyt norm
        err = 1;

    if (!err){
        for (short vertex = 0; vertex < n; ++vertex) {
            if (!color[vertex]) {
                if (!topDFS(adjMatrix, color, vertex, &freeInStack, ansStack, n)) {
                    err = 1;
                    break;
                }
            }
        }
        if (!err){
            for (int i = freeInStack - 1; i >= 0; i--)
                printf("%hd ", ansStack[i]);
        }
    }
    free(ansStack);
    free(color);
    free(adjMatrix);
}
