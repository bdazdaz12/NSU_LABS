#include<stdio.h>
#include<stdlib.h>
#define intMax 2147483647
#define INF 2147483648

char readGraph(unsigned int* adjMatrix, int n, int m){
    for (int i = 0; i < m; i++){
        int from, to;
        long long len;
        if(scanf("%d%d%lld", &from, &to, &len) != 3){
            printf("bad number of lines");
            return 0;
        }
        if (from < 1 || from > n || to < 1 || to > n){
            printf("bad vertex");
            return 0;
        }
        if (len < 0 || len > intMax){
            printf("bad length");
            return 0;
        }
        adjMatrix[(from - 1) * n + (to - 1)] = len;
        adjMatrix[(to - 1) * n + (from - 1)] = len;
    }
    return 1;
}

char prim(unsigned int* adjMatrix, short* ansFrame, int n){
    short freeInFrame = 0;
    char spanTree = 1;
    char* used = malloc(n);
    unsigned int* minEdgeToFrameFrom = malloc(n * sizeof(unsigned int));
    short* endOfMinEdgeFrom = malloc(n * sizeof(short));
    for (int i = 0; i < n; i++) {
        minEdgeToFrameFrom[i] = INF;
        endOfMinEdgeFrom[i] = -1;
        used[i] = 0;
    }

    minEdgeToFrameFrom[0] = 0;
    for (int i = 0; i < n; i++){
        int v = -1;
        for (int j = 0; j < n; j++){
            if (!used[j] && (v == -1 || minEdgeToFrameFrom[j] < minEdgeToFrameFrom[v])) {
                v = j;
            }
        }
        if (minEdgeToFrameFrom[v] == INF){
            printf("no spanning tree");
            spanTree = 0;
            break;
        }
        used[v] = 1;
        if (endOfMinEdgeFrom[v] != -1){ ///не вносим в ответ "ребро" 0 вершины
            ansFrame[freeInFrame] = v;
            ansFrame[(n - 1) + freeInFrame++] = endOfMinEdgeFrom[v];
        }
        for (int to = 0; to < n; to++){
            if (adjMatrix[v * n + to] < minEdgeToFrameFrom[to]) {
                minEdgeToFrameFrom[to] = adjMatrix[v * n + to];
                endOfMinEdgeFrom[to] = v;
            }
        }
    }
    free(used);
    free(minEdgeToFrameFrom);
    free(endOfMinEdgeFrom);
    return spanTree;
}

int main(){
    int n, m;
    if (!scanf("%d%d", &n, &m) || n == 1)
        return 0;
    if (n < 0 || n > 5000) {
        printf("bad number of vertices");
        return 0;
    }
    if (m < 0 || m > n * (n - 1) / 2){
        printf("bad number of edges");
        return 0;
    }

    if (n == 0 || (m == 0 && n != 1) || m < n - 1) {
        printf("no spanning tree");
        return 0;
    }
    unsigned int* adjMatrix = malloc(n * n * sizeof(unsigned int));
    short* ansFrame = malloc((n - 1) * 2 * sizeof(short));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            adjMatrix[i * n + j] = INF;
        }
    }
    char err = 0;
    if (!readGraph(adjMatrix, n, m))
        err = 1;
    if (!err){
        if(prim(adjMatrix, ansFrame, n)){
            for (int i = 0; i < n - 1; i++){
                printf("%d %d\n", ansFrame[i] + 1, ansFrame[(n - 1) + i] + 1);
            }
        }
    }
    free(adjMatrix);
    free(ansFrame);
}
