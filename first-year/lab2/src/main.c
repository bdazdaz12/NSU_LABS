#include <stdio.h>
#include <string.h>

void swap (int *x, int *y){
    int a = *x;
    *x = *y;
    *y = a;
}

int nextPerm(int perm[], int len) {
    for (int i = len - 2; i >= 0; --i) {
        if (perm[i] < perm[i + 1]) {
            int min = i + 1;
            for (int j = len - 1; j > i; --j)
                if (perm[j] < perm[min] && perm[j] > perm[i])
                    min = j;
            swap(&perm[i], &perm[min]);
            for (int k = 1; k <= (len - i - 1) / 2; ++k)
                swap(&perm[i + k], &perm[len - k]);
            return 1;
        }
    }
    return 0;
}

void out(int perm[], int len){
    for (int i = 0; i < len; ++i)
        printf("%d", perm[i]);
    printf("\n");
}

int main() {
    char str[11];
    int count;
    if (!scanf("%10s%d", str, &count))
        return 0;
    int len = strlen(str);
    int perm[len];
    int digits[10] = {0};
    for (int i = 0; i < len; ++i) {
        perm[i] = str[i] - '0';
        digits[perm[i]]++;
        if (str[i] < '0' || str[i] > '9' || digits[perm[i]] > 1){
            printf("bad input");
            return 0;
        }
    }
    for (int i = 0; i < count; ++i){
        if (!nextPerm(perm, len))
            return 0;
        else
            out(perm, len);
    }
}
