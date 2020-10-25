#include<stdio.h>
#include<stdlib.h>
#define intMax 2147483647

typedef struct edge {
    short from;
    short to;
    unsigned int len;
} edge;

char readGraph(edge* edges, int n, int m){
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
        edges[i].from = from - 1;
        edges[i].to = to - 1;
        edges[i].len = len;
    }
    return 1;
}

void swap (edge* x, edge* y){
    edge a = *x;
    *x = *y;
    *y = a;
}

int partition(edge* a, int l, int r){
    edge x = a[(l + r) / 2];
    int i = l;
    int j = r;
    while (i <= j) {
        while (a[i].len < x.len)
            i++;
        while (a[j].len > x.len)
            j--;
        if (i >= j)
            break;
        swap(&a[i++], &a[j--]);
    }
    return j;
}

void quickSort(edge* a, int l, int r){
    if (l < r){
        int q = partition(a, l, r);
        quickSort(a, l, q);
        quickSort(a, q + 1, r);
    }
}

short find(short x, short* parent){
    if (parent[x] == x){
        return x;
    }
    return parent[x] = find(parent[x], parent);
}

void unite(short x, short y, short* parent, short* rank){
    x = find(x, parent);
    y = find(y, parent);
    if (rank[x] < rank[y]) {
        parent[x] = y;
    } else {
        parent[y] = x;
        if (rank[x] == rank[y])
            ++rank[x];
    }
}

int kruskal(edge* edges, int* ansFrame, short* parent, short* rank, int m){
    int frameLen = 0;
    for (int i = 0; i < m; i++){
        if (find(edges[i].from, parent) != find(edges[i].to, parent)){
            unite(edges[i].from, edges[i].to, parent, rank);
            ansFrame[frameLen++] = i;
        }
    }
    return frameLen;
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

    edge* edges = (edge*)malloc(m * sizeof(edge));
    int* ansFrame = malloc(m * sizeof(int));
    short* parent = malloc(n * sizeof(short));
    short* rank = malloc(n * sizeof(short));
    char err = 0;

    if(!readGraph(edges, n, m)) {
        err = 1;
    }
    if (!err){
        quickSort(edges, 0, m - 1);
        for (int i = 0; i < n; i++){
            parent[i] = i;
            rank[i] = 0;
        }
        int frameLen = kruskal(edges, ansFrame, parent, rank, m);
        short bigRoot = find(parent[0], parent);
        for (int i = 1 ; i < n; i++){
            if (bigRoot != find(i, parent)){
                printf("no spanning tree");
                err = 1;
            }
        }
        if(!err){
            for (int i = 0; i < frameLen; i++){
                printf("%d %d\n", edges[ansFrame[i]].from + 1, edges[ansFrame[i]].to + 1);
            }
        }
    }
    free(edges);
    free(parent);
    free(ansFrame);
    free(rank);
}
