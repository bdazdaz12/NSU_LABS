#pragma once

#include "IWorker.h"
#include "IValidator.h"

class ReplaceBlock: public IWorker, IValidator {
private:
    string word1;
    string word2;
public:
    ReplaceBlock(const string& word1, const string& word2){
        this->word1 = word1;
        this->word2 = word2;
    };
    ~ReplaceBlock() override = default;
    bool execute(conveyor& curStage) override;
    bool isValid(conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
