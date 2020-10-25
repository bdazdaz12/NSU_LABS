#include<stdio.h>
#include<string.h>

int getFirstHash(unsigned char* str, int strLen){
    int ans = 0, prevStep = 1;
    for (int i = 0; i < strLen; ++i){
        ans += ((int)str[i] % 3) * prevStep;
        prevStep *= 3;
    }
    return ans;
}

void isEqual(unsigned char* temp, int tempLen, unsigned char* buf, long long link, long long bufCount){
    for (int i = 0; i < tempLen; ++i){
        printf("%lli ", bufCount + link + 1 + i);
        if (buf[link + i] != temp[i])
            break;
    }
}

int main(){
    FILE* inputFile = fopen("in.txt", "r");
    int exp[16] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907};
    unsigned char temp[16];
    int tempLen = 0;
    for (int i = 0; i < 16; ++i) {
        unsigned char a;
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
        unsigned char a;
        if (!fscanf(inputFile, "%c", &a)) {
            fclose(inputFile);
            return 0;
        }
    }
    int tempHash = getFirstHash(temp, tempLen);
    printf("%d ", tempHash);

    unsigned char buf[1024];
    int bufSize = fread(buf, 1, tempLen, inputFile);
    int bufHash = getFirstHash(buf, bufSize);
    int bufHashBegin = ((int)buf[0] % 3);
    bufSize += fread(&buf[tempLen], 1, 1024 - bufSize, inputFile);

    long long bufCount = 0;
    long long link = 0;
    int flag = 0;

    while (bufSize){
        while(link + tempLen - 1 < bufSize) {
            if(flag && (tempLen - 1 < bufSize)){
                bufHash += ((int)buf[tempLen - 1] % 3) * exp[tempLen - 1];
                flag = 0;
            }
            if (tempHash == bufHash)
                isEqual(temp, tempLen, buf, link, bufCount);
            bufHash -= bufHashBegin;
            bufHash /= 3;
            link++;
            bufHashBegin = ((int)buf[link] % 3);
            if(link + tempLen - 1 < bufSize)
                bufHash += ((int)buf[link + tempLen - 1] % 3) * exp[tempLen - 1];
            else
                flag = 1;
        }

        bufSize = 0;
        if (link + tempLen - 1 >= 1024){
            memcpy(buf, &buf[link], tempLen - 1);
            bufSize = fread(&buf[tempLen - 1], 1, link, inputFile) + tempLen - 1;
            bufCount += link;
            link = 0;
            flag = 1;
        }
    }
    fclose(inputFile);
}
