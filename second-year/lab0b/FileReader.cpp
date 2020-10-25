#include "FileReader.h"

FileReader::FileReader(const string &inputFilePath) {
    inputFile.open(inputFilePath);
}

void FileReader::closeFileStream() {
    inputFile.close();
}

bool FileReader::getNextLine(string &outStr){
    return getline(inputFile, outStr)? true : false;
}