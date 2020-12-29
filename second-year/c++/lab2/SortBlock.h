#pragma once

#include "IWorker.h"
#include "IValidator.h"
#include <algorithm>

class SortBlock: public IWorker, IValidator {
public:
    SortBlock() = default;
    ~SortBlock() override = default;
    shared_ptr<IWorker> initialize(const list<string>& input) override{
        if(!input.empty()){
            throw invalid_argument("Wrong input in 'sort' block!");
        }
        return make_shared<SortBlock>();
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
