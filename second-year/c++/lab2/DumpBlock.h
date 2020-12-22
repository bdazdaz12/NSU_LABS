#pragma once

#include "IWorker.h"
#include "IValidator.h"
#include <fstream>

class DumpBlock: public IWorker, IValidator {
private:
    string filename;
    ofstream outputFile;
public:
    DumpBlock() = default;
    explicit DumpBlock(const string& filename) {
        this->filename = filename;
        outputFile.open(filename);
    };
    ~DumpBlock() override {
        outputFile.close();
    };
    shared_ptr<IWorker> initialize(pair<string, string>&& input) override{
        return make_shared<DumpBlock>(input.first);
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
