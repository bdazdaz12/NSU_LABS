#pragma once

#include "IWorker.h"
#include "IValidator.h"

class ReplaceBlock: public IWorker, IValidator {
private:
    string word1;
    string word2;
public:
    ReplaceBlock() = default;
    ReplaceBlock(const string& word1, const string& word2){
        this->word1 = word1;
        this->word2 = word2;
    };
    ~ReplaceBlock() override = default;
    shared_ptr<IWorker> initialize(const list<string>& input) override{
        if (input.size() != 2){
            throw invalid_argument("Wrong input in 'replace' block!");
        }
        return make_shared<ReplaceBlock>(input.front(), input.back());
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
