#include "ReplaceBlock.h"

string ReplaceBlock::isValid(const conveyor& curStage) {
    if (!curStage.haveInput){
        return "Null data input on text replace! Block index: " + to_string(curStage.idx);
    }
    return "";
}

void ReplaceBlock::errorHandler(string &&messages) {
    throw logic_error(messages);
}

void ReplaceBlock::execute(conveyor& curStage) {
    string&& verdict = isValid(curStage);
    if(!verdict.empty()){
        errorHandler(move(verdict));
        //end of work
    }
    curStage.haveOutput = true;
    curStage.output = curStage.input;
    if(curStage.input.empty()){
        return;
    }
    for (auto& curStr: curStage.output){
        size_t entry;
        while ((entry = curStr.find(word1)) != string::npos){
            curStr.replace(entry, word1.size(), word2);
        }
    }
}
