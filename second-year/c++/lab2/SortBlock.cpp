#include "SortBlock.h"

string SortBlock::isValid(const conveyor &curStage) {
    if (!curStage.haveInput){
        return "Null input on sort! Block index: " + to_string(curStage.idx);
    }
    return "";
}

void SortBlock::errorHandler(string &&messages) {
    throw logic_error(messages);
}

void SortBlock::execute(conveyor& curStage) {
    string&& verdict = isValid(curStage);
    if(!verdict.empty()){
        errorHandler(move(verdict));
        //end of work
    }
    curStage.haveOutput = true;
    *curStage.output = *curStage.input;
    if (curStage.input->empty()) {
        return;
    }
    sort(curStage.output->begin(), curStage.output->end());
}