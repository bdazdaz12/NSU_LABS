#include<stdio.h>
#include<stdlib.h>


char readGraph(int* adjMatrix, int n, int m){
    for (int i = 0 ; i < m; ++i){
        int from, to;
        long long len; ////tyt mogyt bit trabls
        if(scanf("%d%d%lld", &from, &to, &len) != 3){
            printf("bad number of lines");
            return 0;
        }
        if (from < 1 || from > n || to < 1 || to > n){
            printf("bad vertex");
            return 0;
        }
        if (len < 0 || len > 2147483647){
            printf("bad length");
            return 0;
        }
        adjMatrix[(from - 1) * n + (to - 1)] = len;
        adjMatrix[(to - 1) * n + (from - 1)] = len;////zabil eto dobavit
    }
    return 1;
}

int findMinNextVertex(char* used, unsigned int* dist, int n){
    int ans = -1;
    for (int i = 0 ; i < n; i++){
        if (!used[i] && (ans == -1 || dist[i] < dist[ans])){
            ans = i;
        }
    }
    return ans;
}

void dijkstra(int* adjMatrix, char* used, unsigned int* dist, int n, int s, short* path, char* countOfWays){
    dist[s] = 0;
    countOfWays[s] = 1;
    for (int i = 0; i < n; i++){
        int v = findMinNextVertex(used, dist, n);
        if (dist[v] == 4294967295)
            break;
        used[v] = 1;
        for (int j = 0; j < n; j++){ //////tyt dich mozhet
            if(adjMatrix[v * n + j] != -1){
                if (countOfWays[j] < 3)
                    countOfWays[j] += countOfWays[v];
                if ((long long)dist[v] + (long long)adjMatrix[v * n + j] < (long long)dist[j]){
                    path[j] = v;
                    if ((long long)dist[v] + (long long)adjMatrix[v * n + j] > 2147483647){
                        dist[j] = 2147483648;
                    }
                    else {
                        dist[j] = dist[v] + adjMatrix[v * n + j];
                    }
                }
            }
        }
    }
}

void findWay(unsigned int* dist, int s, int f, short* path, char* countOfWays){
    if (dist[f] == 4294967295){
        printf("no path");
        return;
    }
    if (dist[f] > 2147483647 && countOfWays[f] >= 2){
        printf("overflow");
        return;
    }
    printf("%d ", f + 1);
    int p = f;
    while(p != s){
        p = path[p];
        printf("%d ", p + 1);
    }
}

int main(){
    int n, s, f, m;
    char err = 0;
    if (!scanf("%d%d%d%d", &n, &s, &f, &m))
        return 0;
    if (n < 0 || n > 5000){
        printf("bad number of vertices");
        return 0;
    }
    if (m < 0 || m > n * (n + 1) / 2){ ////tyt sporno
        printf("bad number of edges");
        return 0;
    }
    if (s < 1 || s > n || f < 1 || f > n){
        printf("bad vertex");
        return 0;
    }
    int* adjMatrix = malloc(n * n * sizeof(int));
    unsigned int* dist = malloc(n * sizeof(int));
    short* path = malloc(2 * n);
    char* countOfWays = malloc(n); ///nekrasivo
    char* used = malloc(n);
    for (int i = 0; i < n; i++){
        countOfWays[i] = 0;
        used[i] = 0;
        dist[i] = 4294967295;
        for (int j = 0; j < n; j++){
            adjMatrix[i * n + j] = -1;
        }
    }
    if (!readGraph(adjMatrix, n, m))
        err = 1;
    if (!err){
        dijkstra(adjMatrix, used, dist, n, s - 1, path, countOfWays);///
        for (int i = 0; i < n; i++){
            if (dist[i] == 4294967295){
                printf("oo ");
            }
            else if (dist[i] == 2147483648){
                printf("INT_MAX+ ");
            }
            else{
                printf("%u ", dist[i]);
            }
        } ////tyt zbs vrode
        printf("\n");
        findWay(dist, s - 1, f - 1, path, countOfWays);
    }
    free(adjMatrix);
    free(used);
    free(dist);
    free(path);
    free(countOfWays);
}
