#pragma once

#include "IWorker.h"
#include "IValidator.h"

class SortBlock: public IWorker, IValidator {
public:
    SortBlock() = default;
    ~SortBlock() override = default;
    bool execute(vector<string> *inputData, vector<string> *outputData, bool input, bool output, int idx) override;
    bool isValid() override;
};
