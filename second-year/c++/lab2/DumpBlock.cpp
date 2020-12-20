#include "DumpBlock.h"

string DumpBlock::isValid(const conveyor &curStage) {
    if (!curStage.haveInput){
        return "logic_error: Null input on Dump! Block index: " + to_string(curStage.idx);
    }
    if (!outputFile.good()){
        return "invalid_argument: Bad output file: " + filename;
    }
    return "";
}

void DumpBlock::errorHandler(string &&messages) {
    throw runtime_error(messages);
}

void DumpBlock::execute(conveyor &curStage) {
    string&& verdict = isValid(curStage);
    if(!verdict.empty()){
        errorHandler(move(verdict));
        //end of work
    }
    curStage.haveOutput = true;
    curStage.output = curStage.input;
    for (const auto& str: curStage.input){
        outputFile << str;
    }
}