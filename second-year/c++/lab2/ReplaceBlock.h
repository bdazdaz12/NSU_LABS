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
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
