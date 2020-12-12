#pragma once

#include "IWorker.h"
#include "IValidator.h"

class GrepBlock: IWorker, IValidator {
public:
    GrepBlock() = default;
    ~GrepBlock() override = default;
    bool execute(vector<string> *inputData, vector<string> *outputData, bool input, bool output, int idx) override;
    bool isValid() override;
};
