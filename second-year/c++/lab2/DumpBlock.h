#pragma once

#include "IWorker.h"
#include "IValidator.h"

class DumpBlock: public IWorker, IValidator {
public:
    DumpBlock() = default;
    ~DumpBlock() override = default;
    bool execute(vector<string> *inputData, vector<string> *outputData, bool input, bool output, int idx) override;
    bool isValid() override;
};
