#include <stdio.h>
#include <malloc.h>

void swap (int *x, int *y){
    int a = *x;
    *x = *y;
    *y = a;
}

int Left(int i){
    return 2*i + 1;
}

int Right(int i){
    return 2*i + 2;
}

void minHeapify (int *a, int i, int heapSize){
    int l = Left(i);
    int r = Right(i);
    int least;
    if (l < heapSize && a[l] < a[i])
        least = l;
    else
        least = i;
    if (r < heapSize && a[r] < a[least])
        least = r;
    if (least != i){
        swap(&a[i], &a[least]);
        minHeapify(a, least, heapSize);
    }
}

void buildMinHeap(int *a, int heapSize){
    for (int i = heapSize >> 1; i >= 0; --i){
        minHeapify(a, i, heapSize);
    }
}

void heapsort(int *a, int heapSize){
    buildMinHeap(a, heapSize);
    for (int i = heapSize - 1; i >= 0; --i){
        printf("%d ", a[0]);
        swap(&a[0], &a[i]);
        heapSize--;
        minHeapify(a, 0, heapSize);
    }
}

int main() {
    int n;
    if (!scanf("%d", &n))
        return 0;
    int heapSize = n;
    int *a = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i){
        if (!scanf("%d", &a[i]))
            return 0;
    }
    heapsort(a, heapSize);
    free(a);
    return 0;
}

