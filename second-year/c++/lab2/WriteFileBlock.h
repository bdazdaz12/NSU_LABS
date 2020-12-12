#pragma once

#include "IWorker.h"
#include "IValidator.h"

class WriteFileBlock: public IWorker, IValidator {
protected:
    bool isValid() override;
public:
    bool execute(vector<string> *inputData, vector<string> *outputData, bool input, bool output, int idx) override;
};
