#include <stdio.h>
#include <stdlib.h>

void swap (int *x, int *y){
    int a = *x;
    *x = *y;
    *y = a;
}

int partition(int *a, int l, int r){
    int x = a[(l + r) / 2];
    int i = l;
    int j = r;
    while (i <= j) {
        while (a[i] < x)
            i++;
        while (a[j] > x)
            j--;
        if (i >= j)
            break;
        swap(&a[i++], &a[j--]);
    }
    return j;
}

void quickSort(int *a, int l, int r){
    if (l < r){
        int q = partition(a, l, r);
        quickSort(a, l, q);
        quickSort(a, q + 1, r);
    }
}

void printArray(int *a, int n){
    for (int i = 0; i < n; ++i)
        printf("%d ", a[i]);
}

int main() {
    int n;
    if (!scanf("%d", &n))
        return 0;
    int *a = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i){
        if (!scanf("%d", &a[i]))
            return 0;
    }
    quickSort(a, 0, n - 1);
    printArray(a, n);
    free(a);
    return 0;
}
