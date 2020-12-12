#pragma once

#include "IWorker.h"
#include "IValidator.h"

class ReadFileBlock: public IWorker, IValidator {
public:
    ReadFileBlock() = default;
    ~ReadFileBlock() override = default;
    bool execute(vector<string> *inputData, vector<string> *outputData, bool input, bool output, int idx) override;
    bool isValid() override;
};
