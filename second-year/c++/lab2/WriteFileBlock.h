#pragma once

#include "IWorker.h"
#include "IValidator.h"
#include <fstream>

class WriteFileBlock: public IWorker, IValidator {
private:
    string filename;
    ofstream outputFile;
public:
    explicit WriteFileBlock(const string& filename) {
        this->filename = filename;
        outputFile.open(filename);
    };
    ~WriteFileBlock() override {
        outputFile.close();
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
