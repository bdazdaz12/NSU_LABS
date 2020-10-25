#include "ReportService.h"

ReportService::ReportService(const std::string &outputFilePath) {
    outputFile.open(outputFilePath);
}

void ReportService::writeInFile(const std::vector<pair<string, pair<unsigned int, double>>>&fileStat) {
    for(const auto& pair: fileStat){
        outputFile << pair.first << ", " << pair.second.first << ", "
                   << pair.second.second << std::endl;
    }
}

void ReportService::closeFileStream() {
    outputFile.close();
}