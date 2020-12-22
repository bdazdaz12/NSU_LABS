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
    shared_ptr<IWorker> initialize(pair<string, string>&& input) override{
        return make_shared<GrepBlock>(input.first);
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
