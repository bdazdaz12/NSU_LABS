#include "WriteFileBlock.h"

string WriteFileBlock::isValid(const conveyor &curStage) {
    if (!curStage.haveInput){
        return "logic_error: Null input on Dump! Block index: " + to_string(curStage.idx);
    }
    if (!outputFile.good()){
        return "invalid_argument: Bad output file: " + filename;
    }
    return "";
}

void WriteFileBlock::errorHandler(string &&messages) {
    throw runtime_error(messages);
}

void WriteFileBlock::execute(conveyor &curStage) {
    string&& verdict = isValid(curStage);
    if(!verdict.empty()){
        errorHandler(move(verdict));
        //end of work
    }
    curStage.haveOutput = false;
    for (const auto& str: *curStage.input){
        outputFile << str << endl;
    }
}