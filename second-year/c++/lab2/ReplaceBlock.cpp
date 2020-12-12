#include "ReplaceBlock.h"

bool ReplaceBlock::isValid(conveyor& curStage) {
    if (!curStage.haveInput){
        return false;
    }
    return true;
}

void ReplaceBlock::errorHandler(string &&messages) {
    throw logic_error(messages);
}

bool ReplaceBlock::execute(conveyor& curStage) {
    if(!isValid(curStage)){
        errorHandler("Null data input on text replace! Block index: " + to_string(curStage.idx));
        //end of work
    }
    curStage.output = curStage.input;
    if(curStage.input.empty()){
        return true;
    }
    for (auto& curStr: curStage.output){
        size_t entry;
        while ((entry = curStr.find(word1)) != string::npos){
            curStr.replace(entry, word1.size(), word2);
        }
    }
    curStage.haveOutput = true;
    return true;
}
