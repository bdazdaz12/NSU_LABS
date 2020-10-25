#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

typedef struct node{
    unsigned char sym;
    short pathSize;
    long long weight;
    char* hufCode;
    struct node* father;
    struct node* rightSon;
    struct node* leftSon;
} node;

short readInBuffer(unsigned char* buffer, FILE* inputFile){
    return fread(buffer, 1, 1024, inputFile);
}

void swap (node* x, node* y){
    node a = *x;
    *x = *y;
    *y = a;
}

int partition(node* a, int l, int r){
    long long x = a[(l + r) / 2].weight;
    int i = l;
    int j = r;
    while (i <= j) {
        while (a[i].weight < x)
            i++;
        while (a[j].weight > x)
            j--;
        if (i >= j)
            break;
        swap(&a[i++], &a[j--]);
    }
    return j;
}

void quickSort(node* a, int l, int r){
    if (l < r){
        int q = partition(a, l, r);
        quickSort(a, l, q);
        quickSort(a, q + 1, r);
    }
}

void giveCode(node* a, node* b, node* father){
    node* min = a->weight < b->weight ? a : b;
    node* max = a->weight < b->weight ? b : a;
    min->father = father;
    max->father = father;
    father->leftSon = max;
    father->rightSon = min;
}

node* buildHuffmanTree(node* a, node* b, int dictionarySize){
    for (int i = 0; i < dictionarySize - 1; ++i){
        b[i].weight = UINT_MAX;
        b[i].father = NULL;
        b[i].rightSon = NULL;
        b[i].leftSon = NULL;
    }
    int i = 0, j = 0;
    for (int k = 0; k < dictionarySize - 1; ++k) {
        long long aa = 0, ab = 0;
        long long bb = LLONG_MAX; ////если у меня 2 ноды, то это ломаеться
        if (j + 1 < dictionarySize - 1){
            bb = b[j].weight + b[j + 1].weight;
        }
        if (i + 1 < dictionarySize) {
            aa = a[i].weight + a[i + 1].weight;
        }
        if(i < dictionarySize){
            ab = a[i].weight + b[j].weight;
        }
        if (aa && aa <= ab && aa <= bb){
            b[k].weight = aa;
            giveCode(&a[i + 1], &a[i], &b[k]);
            i += 2;
        }
        else if (ab && (!aa || ab <= aa) && ab <= bb){
            b[k].weight = ab;
            giveCode(&b[j], &a[i],&b[k]);
            i++; j++;
        }
        else{
            b[k].weight = bb;
            giveCode(&b[j + 1], &b[j], &b[k]);
            j += 2;
        }
    }
    return &b[dictionarySize - 2];
}

node* makeTree(node* dictionary, int dictionarySize, node* sumNodes, int* arrWeightOfSym){
    int freeInDictionary = 0;
    for (int i = 0; i < 256; ++i){
        if (arrWeightOfSym[i]) {
            dictionary[freeInDictionary].sym = i;
            dictionary[freeInDictionary].weight = arrWeightOfSym[i];
            dictionary[freeInDictionary].hufCode = (char*)malloc(dictionarySize + 1);////самый длинный код --- +1
            dictionary[freeInDictionary].father = NULL;
            dictionary[freeInDictionary].rightSon = NULL;
            dictionary[freeInDictionary++].leftSon = NULL;
        }
    }
    node* root;
    if (dictionarySize == 1){
        root = (node*)malloc(sizeof(node));
        root->leftSon = dictionary;
        root->rightSon = NULL;
        root->father = NULL;
        dictionary->father = root;
        return root;
    }
    quickSort(dictionary, 0, dictionarySize - 1);
    root = buildHuffmanTree(dictionary, sumNodes, dictionarySize);
    return root;
}

void searchOptimization(node* dictionary, int dictionarySize, int* arrWeightOfSym){
    for (int i = 0; i < dictionarySize; ++i) {
        dictionary[i].weight += i;
        arrWeightOfSym[dictionary[i].sym] += i;
    }
}

void checkBuffer(FILE* outputFile, unsigned char* outBuf, short* outBufSize){
    if (*outBufSize == 1024){
        fwrite(outBuf, 1, 1024, outputFile);
        *outBufSize = 0;
    }
}

void checkZipByte(FILE* outputFile, unsigned char* outBuf, short* outBufSize, char* countOfDigits, unsigned char* nextZipByte){
    if (*countOfDigits == 8){
        outBuf[(*outBufSize)++] = *nextZipByte;
        checkBuffer(outputFile, outBuf, outBufSize);
        *nextZipByte = 0;
        *countOfDigits = 0;
    }
}

void treeCrawl(FILE* outputFile, unsigned char* outBuf, short* outBufSize, node* curNode, char* countOfDigits, unsigned char* nextZipByte, short deep){
    checkZipByte(outputFile, outBuf, outBufSize, countOfDigits, nextZipByte);
    if(curNode->leftSon) {
        (*countOfDigits)++;
        (*nextZipByte) <<= 1;
        (*nextZipByte)++; ///pishem 1
        treeCrawl(outputFile, outBuf, outBufSize, curNode->leftSon, countOfDigits, nextZipByte, deep + 1);
    }
    if (curNode->rightSon){
        treeCrawl(outputFile, outBuf, outBufSize, curNode->rightSon, countOfDigits, nextZipByte, deep + 1);
    }
    if(!curNode->leftSon && !curNode->rightSon) {
        (*countOfDigits)++;
        (*nextZipByte) <<= 1; ///пишем 0 как поднятие вверх

        (*nextZipByte) <<= (8 - *countOfDigits);
        (*nextZipByte) += (curNode->sym >> (*countOfDigits));
        outBuf[(*outBufSize)++] = *nextZipByte;  ////не должно ломать checkbuffer потому что максимальный обход < 1 buf
        *nextZipByte = *countOfDigits == 8? curNode->sym : curNode->sym & ((1 << *countOfDigits) - 1);

        curNode->pathSize = deep;
    }
    checkZipByte(outputFile, outBuf, outBufSize, countOfDigits, nextZipByte);
}

void createTable(node* dictionary, int dictionarySize){
    for (int i = 0; i < dictionarySize; ++i){
        int idx = 1;
        node* curSym = &dictionary[i];
        node* curNode = curSym;
        node* prevNode = curNode;
        while(curNode->father){
            curNode = curNode->father;
            curSym->hufCode[curSym->pathSize - idx++] = curNode->leftSon == prevNode? 0 : 1;
            prevNode = curNode;
        }
    }
}

node* binSearchSymInDictionary(node* dictionary, int dictionarySize, int* arrWeightOfSym, unsigned char sym){
    int left = -1;
    int right = dictionarySize;
    while(left < right - 1){
        int middle = (left + right) >> 1;
        if (dictionary[middle].weight < arrWeightOfSym[sym])
            left = middle;
        else
            right = middle;
    }
    return &dictionary[right];
}

void encryption(FILE* inputFile, FILE* outputFile, node* dictionary, int dictionarySize, int* arrWeightOfSym,unsigned char* inBuf, unsigned char* outBuf, short* outBufSize, unsigned char* nextZipByte, char* countOfDigits){
    unsigned char LastByte = 128;
    if (dictionarySize == 1)
        LastByte = 0;
    short inBufSize;
    node* nextSymToZip;
    while((inBufSize = readInBuffer(inBuf, inputFile))){
        for(int i = 0; i < inBufSize; ++i){
            nextSymToZip = binSearchSymInDictionary(dictionary, dictionarySize, arrWeightOfSym, inBuf[i]);
            for (int j = 0; j < nextSymToZip->pathSize; j++){
                (*countOfDigits)++;
                *nextZipByte <<= 1;
                *nextZipByte += nextSymToZip->hufCode[j];
                if (*countOfDigits == 8) {
                    checkZipByte(outputFile, outBuf, outBufSize, countOfDigits, nextZipByte);
                }
            }
        }
    }
    if (*countOfDigits){
        LastByte += *countOfDigits;
        (*nextZipByte) <<= (8 - *countOfDigits);
        *countOfDigits = 8;
        checkZipByte(outputFile, outBuf, outBufSize, countOfDigits, nextZipByte);
    } else {
        LastByte += 8;
    }
    outBuf[(*outBufSize)++] = LastByte;
    fwrite(outBuf, 1, *outBufSize, outputFile);
}

void zip(FILE* inputFile, FILE* outputFile){
    int arrWeightOfSym[256] = {0};
    int dictionarySize = 0;
    unsigned char* inBuf = (unsigned char*)malloc(1024);
    short inBufSize;
    while((inBufSize = readInBuffer(inBuf, inputFile))){
        for (int i = 0; i < inBufSize; ++i){
            if (!arrWeightOfSym[inBuf[i]]) {
                dictionarySize++;
            }
            arrWeightOfSym[inBuf[i]]++;
        }
    }
    if (dictionarySize == 0) {  /////net sym
        free(inBuf);
        return;
    }
    fseek(inputFile, 3, SEEK_SET);
    node* dictionary = (node*)malloc(sizeof(node) * dictionarySize);
    node* sumNodes = (node*)malloc(sizeof(node) * (dictionarySize - 1));

    node* root = makeTree(dictionary, dictionarySize, sumNodes, arrWeightOfSym);
    searchOptimization(dictionary, dictionarySize, arrWeightOfSym);
    unsigned char* outBuf = (unsigned char*)malloc(1024);
    short outBufSize = 0;
    unsigned char nextZipByte = 0;
    char countOfDigits = 0;
    treeCrawl(outputFile, outBuf, &outBufSize, root, &countOfDigits, &nextZipByte, 0);
    createTable(dictionary, dictionarySize);
    encryption(inputFile, outputFile, dictionary, dictionarySize, arrWeightOfSym, inBuf, outBuf, &outBufSize, &nextZipByte, &countOfDigits);

    if (dictionarySize == 1)
        free(root);
    free(sumNodes);
    for(int i = 0; i < dictionarySize; ++i)
        free(dictionary[i].hufCode);
    free(dictionary);
    free(inBuf);
    free(outBuf);
}
/// ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP  ZIP

unsigned char getNextBit(FILE* inputFile, unsigned char* inBuf, short* inBufSize, short* bytePointer, int* numOfCurByte, char* currentBitDigit){
    if (*currentBitDigit == 8){
        (*numOfCurByte)++;
        (*bytePointer)++;
        if (*bytePointer == 1024){
            *inBufSize = readInBuffer(inBuf, inputFile);
            *bytePointer = 0;
        }
        *currentBitDigit = 1;
        return (inBuf[*bytePointer] >> 7);
    } else {
        (*currentBitDigit)++;
        return (inBuf[*bytePointer] >> (8 - *currentBitDigit)) & 1;
    }
}

node* addNodeInTree(node* father){
    node* newNode = (node*)malloc(sizeof(node));
    newNode->father = father;
    newNode->leftSon = NULL;
    newNode->rightSon = NULL;
    return newNode;
}

node* rebuildTree(FILE* inputFile, unsigned char* inBuf, short* inBufSize, short* bytePointer, int* numOfCurByte, char* currentBitDigit, unsigned char haveRightSon){
    *currentBitDigit = 0; /////nezhno
    node* root = (node*)malloc(sizeof(node));
    root->father = NULL;
    root->leftSon = NULL;
    root->rightSon = NULL;
    node* curNode = root;
    node* prevNode;

    while(!(curNode == root && root->leftSon && root->rightSon) && !(!haveRightSon && curNode == root && root->leftSon)){
        unsigned char currentBit = getNextBit(inputFile, inBuf, inBufSize, bytePointer, numOfCurByte, currentBitDigit);
        if (currentBit == 1){
            curNode->leftSon = addNodeInTree(curNode);
            curNode = curNode->leftSon;
        } else if (currentBit == 0){
            (*numOfCurByte)++;
            curNode->sym = (inBuf[*bytePointer] << *currentBitDigit) + (inBuf[*bytePointer + 1] >> (8 - *currentBitDigit));
            (*bytePointer)++;

            prevNode = curNode;
            curNode = curNode->father;
            while(prevNode == curNode->rightSon && curNode != root){
                prevNode = curNode;
                curNode = curNode->father;
            }
            if (haveRightSon && prevNode == curNode->leftSon){
                curNode->rightSon = addNodeInTree(curNode);
                curNode = curNode->rightSon;
            }
        }
    }
    return root;
}

void decryption(FILE* inputFile, FILE* outputFile, unsigned char* inBuf, short* inBufSize, short* bytePointer, char* currentBitDigit, node* root, int sizeOfFile, int* numOfCurByte, short numOfBitsInLastByte){
    unsigned char* outBuf = (unsigned char*)malloc(1024);
    short outBufSize = 0;
    node* currentNode = root;
    unsigned char currentBit = getNextBit(inputFile, inBuf, inBufSize, bytePointer, numOfCurByte, currentBitDigit);
    while(*numOfCurByte < sizeOfFile - 1 || (*currentBitDigit <= numOfBitsInLastByte && *numOfCurByte < sizeOfFile)){
        currentNode = currentBit == 0? currentNode->leftSon : currentNode->rightSon;
        if (!currentNode->leftSon && !currentNode->rightSon){
            outBuf[outBufSize++] = currentNode->sym;
            currentNode = root;
            if (outBufSize == 1024){
                checkBuffer(outputFile, outBuf, &outBufSize);
            }
        }
        currentBit = getNextBit(inputFile, inBuf, inBufSize, bytePointer, numOfCurByte, currentBitDigit);
    }
    if (outBufSize) {
        fwrite(outBuf, 1, outBufSize, outputFile);
    }
    free(outBuf);
}

void dfsFreeTree(node* root){
    if(root->leftSon){
        dfsFreeTree(root->leftSon);
    }
    if(root->rightSon){
        dfsFreeTree(root->rightSon);
    }
    free(root);
}

void unZip(FILE* inputFile,  FILE* outputFile){
    fseek(inputFile,-1, SEEK_END);
    short numOfBitInLastByte = fgetc(inputFile);
    unsigned char haveRightSon = (numOfBitInLastByte >> 7);
    numOfBitInLastByte &= 127;
    int sizeOfFile = ftell(inputFile) - 3; ////-3
    if(!sizeOfFile)
        return;
    fseek(inputFile, 3, SEEK_SET); ////3

    unsigned char* inBuf = (unsigned char*)malloc(1024);
    short inBufSize = readInBuffer(inBuf, inputFile);
    short bytePointer = 0;
    char currentBitDigit = 0;
    int numOfCurByte = 1;

    node* root = rebuildTree(inputFile, inBuf, &inBufSize, &bytePointer, &numOfCurByte, &currentBitDigit, haveRightSon);
    decryption(inputFile, outputFile, inBuf, &inBufSize, &bytePointer, &currentBitDigit, root, sizeOfFile, &numOfCurByte, numOfBitInLastByte);
    dfsFreeTree(root);
    free(inBuf);
}

int main(){
    FILE* inputFile = fopen("in.txt", "rb");
    FILE* outputFile = fopen("out.txt", "wb");
    unsigned char mode[4];
    if (!fread(mode, 1, 3, inputFile)) {
        fclose(inputFile);
        fclose(outputFile);
        return 0;
    }
    if (mode[0] == 'c'){
        zip(inputFile, outputFile);
    } else if (mode[0] == 'd'){
        unZip(inputFile, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
}
