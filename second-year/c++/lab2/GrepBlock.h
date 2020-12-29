#pragma once

#include "IWorker.h"
#include "IValidator.h"

class GrepBlock: public IWorker, IValidator {
private:
    string word;
public:
    GrepBlock() = default;
    explicit GrepBlock(const string& word) {
        this->word = word;
    };
    ~GrepBlock() override = default;
    shared_ptr<IWorker> initialize(const list<string>& input) override{
        if (input.empty()){
            throw invalid_argument("Not enough input in 'grep' block!");
        }
        return make_shared<GrepBlock>(input.front());
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
