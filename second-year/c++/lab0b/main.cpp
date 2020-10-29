#include <string>
#include <gtest/gtest.h>
#include "FileReader.h"
#include "WordStatService.h"
#include "StringParser.h"
#include "ReportService.h"

using namespace std;

int main(int argc, char** argv){
    FileReader fileReader(argv[1]);
    WordStatService wordStatService;
    StringParser stringParser;
    string nextLine;
    while(fileReader.getNextLine(nextLine)){
        for (const auto& word: stringParser.parseString(nextLine)){
            wordStatService.putWordInSet(word);
        }
    }
    ReportService reportService(argv[2]);
    reportService.writeInFile(wordStatService.getStatistic());
    reportService.closeFileStream();
}

//int main(int argc, char** argv) {
//    return RUN_ALL_TESTS();
//}
