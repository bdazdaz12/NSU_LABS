#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main() {
    char inputStr[14];
    char outIntPart[61], outFracPart[61];
    char digits[] = "0123456789abcdef";
    int b1, b2, i, dotCounts = 0, frac = 0;
    int scannedCounts;
    scannedCounts = scanf("%d%d%13s", &b1, &b2, inputStr);
    if (scannedCounts != 3)
       return 0;
    int lenInputStr = strlen(inputStr);
    for (i = 0; i < lenInputStr; i++) {
        if (inputStr[i] == '.')
            dotCounts++;
        else if (llabs(strchr(digits, tolower(inputStr[i])) - digits) >= b1)
            break;
    }
    if (dotCounts > 1 || inputStr[0] == '.' || inputStr[lenInputStr - 1] == '.' ||
        b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16 || i < lenInputStr - 1){
        printf("bad input");
        return 0;
    }
    int sizeIntPart;
    if (dotCounts == 0) {
        sizeIntPart = lenInputStr;
    }
    else{
        frac = 1;
        sizeIntPart = strchr(inputStr, '.') - inputStr;
    }
    long long intPart = 0;
    for (i = 0; i < sizeIntPart; i++)
        intPart = intPart * b1 + (strchr(digits, tolower(inputStr[i])) - digits);
    int indexIntPart = 0;
    while (intPart != 0) {
        int a = intPart % b2;
        outIntPart[indexIntPart++] = digits[a];
        intPart = (intPart - a) / b2;
    }
    double fracPart = 0;
    for (i = lenInputStr - 1; i > sizeIntPart; i--)
        fracPart = (fracPart + (strchr(digits, tolower(inputStr[i])) - digits)) / b1;
    int indexFracPart = 0;
    if (frac) {
        while (indexFracPart < 12) {
            double  a = fracPart * b2;
            outFracPart[indexFracPart++] = digits[(int) a];
            fracPart = fracPart * b2 - floor(a);
        }
    }
    if (indexIntPart == 0)
        printf("0");
    else
        for (i = indexIntPart - 1; i >= 0; i--)
            printf("%c", outIntPart[i]);
    if (frac) {
        printf(".");
        for (i = 0; i < indexFracPart; i++)
            printf("%c", outFracPart[i]);
    }
}
