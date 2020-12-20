#pragma once

#include "IWorker.h"
#include "IValidator.h"
#include <fstream>

class DumpBlock: public IWorker, IValidator {
private:
    string filename;
    ofstream outputFile;
public:
    explicit DumpBlock(const string& filename) {
        this->filename = filename;
        outputFile.open(filename);
    };
    ~DumpBlock() override {
        outputFile.close();
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
