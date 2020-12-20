#include "GrepBlock.h"

string GrepBlock::isValid(const conveyor &curStage) {
    if(!curStage.haveInput){
        return "Null input on grep! Block index: " + to_string(curStage.idx);
    }
    return "";
}

void GrepBlock::errorHandler(string &&messages) {
    throw logic_error(messages);
}

void GrepBlock::execute(conveyor &curStage) { //TODO: тут возможно придется изменить input в блок
    string&& verdict = isValid(curStage);
    if(!verdict.empty()){
        errorHandler(move(verdict));
        //end of work
    }
    curStage.haveOutput = true;
    if (curStage.input.empty()){
        curStage.output = curStage.input;
        return;
    }
    curStage.output = vector<string>();
    for (const auto& str: curStage.input){
        if (str.find(word) != string::npos){
            curStage.output.push_back(str);
        }
    }
}