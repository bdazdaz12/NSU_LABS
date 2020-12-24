#include "ReadFileBlock.h"

string ReadFileBlock::isValid(const conveyor &curStage) {
    if (curStage.haveInput){
        return "logic_error: We can't have input here! Block index: " + to_string(curStage.idx);
    }
    if (!inputFile.good()){
        return "invalid_argument: Bad input file " + filename;
    }
    return "";
}

void ReadFileBlock::errorHandler(string &&messages) {
    throw runtime_error(messages);
}

void ReadFileBlock::execute(conveyor &curStage) {
    string&& verdict = isValid(curStage);
    if(!verdict.empty()){
        errorHandler(move(verdict));
        //end of work
    }
    string nextLine;
    curStage.haveOutput = true;
    while (getline(inputFile, nextLine)){
        curStage.output->push_back(nextLine);
    }
}