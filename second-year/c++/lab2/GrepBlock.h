#pragma once

#include "IWorker.h"
#include "IValidator.h"

class GrepBlock: IWorker, IValidator {
private:
    string word;
public:
    explicit GrepBlock(const string& word) {
        this->word = word;
    };
    ~GrepBlock() override = default;
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
