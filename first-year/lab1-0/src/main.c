#include<stdio.h>
#include<string.h>

void creatArrOfShift(int *arrOfShift, char *temp, int tempLen){
    for (int i = 0; i < 256; i++)
        arrOfShift[i] = tempLen;
    for (int i = 0; i < tempLen - 1; ++i){
        arrOfShift[(int)temp[i]] = tempLen - 1 - i;
    }
}

int main(){
    FILE* inputFile = fopen("in.txt", "r");
    char temp[16];
    int tempLen = 0;
    for (int i = 0; i < 16; ++i) {
        char a;
        if (!fscanf(inputFile, "%c", &a)) {
            fclose(inputFile);
            return 0;
        }
        if (a == '\n') {
            tempLen = i;
            break;
        }
        temp[i] = a;
    }
    if (!tempLen){
        tempLen = 16;
        char a;
        if (!fscanf(inputFile, "%c", &a)) {
            fclose(inputFile);
            return 0;
        }
    }

    int arrOfShift[256];
    creatArrOfShift(arrOfShift, temp, tempLen);
    char buf[8192];
    int link = tempLen - 1, bufSize;
    bufSize = fread(buf, 1, 8192, inputFile);
    long long bufCount = 0;
    while (bufSize){
        while(link < bufSize) {
            int flag = 0;
            for (int i = tempLen - 1; i >= 0; --i) {
                int nextInBuf = link - tempLen + 1 + i;
                printf("%lli ", bufCount + nextInBuf + 1);
                if (temp[i] == buf[nextInBuf]) {
                    flag = 1;
                    if (!i) {
                        link += tempLen;
                        break;
                    }
                }
                else if (!flag) {
                    link += arrOfShift[(int)buf[nextInBuf]];
                    break;
                }
                else {
                    link += arrOfShift[(int)temp[tempLen - 1]];
                    break;
                }
            }
        }
        bufSize = 0;
        if (link >= 8192){
            int idx = link - tempLen + 1;
            if (link < 8191 + tempLen)
                memcpy(buf, &buf[idx], (8192 - idx));
            bufSize = fread(&buf[8192 - idx], 1, idx, inputFile);
            link = tempLen - 1;
            bufCount += 8192;
        }
    }
    fclose(inputFile);
}
