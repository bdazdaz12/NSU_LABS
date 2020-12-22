#pragma once

#include "IWorker.h"
#include "IValidator.h"
#include <fstream>

class WriteFileBlock: public IWorker, IValidator {
private:
    string filename;
    ofstream outputFile;
public:
    WriteFileBlock() = default;
    explicit WriteFileBlock(const string& filename) {
        this->filename = filename;
        outputFile.open(filename);
    };
    ~WriteFileBlock() override {
        outputFile.close();
    };
    shared_ptr<IWorker> initialize(pair<string, string>&& input) override{
        return make_shared<WriteFileBlock>(input.first);
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
