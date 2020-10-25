#pragma once
#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

class ReportService {
public:
    ReportService() = default;
    ReportService(const std::string& outputFilePath);
    void writeInFile(const vector<pair<string, pair<unsigned int, double>>>& fileStat);
    void closeFileStream();
private:
    std::ofstream outputFile;
};
