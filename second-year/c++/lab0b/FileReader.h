#pragma once
#include <fstream>

using namespace std;

class FileReader {
public:
    FileReader() = default;
    FileReader(const string &inputFilePath);
    void closeFileStream();
    bool getNextLine(string &outStr);
private:
    ifstream inputFile;
};