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
    shared_ptr<IWorker> initialize(const list<string>& input) override{
        if (input.empty()){
            throw invalid_argument("Not enough input in 'dump' block!");
            //TODO: можно конечно бы и винести это прокрасивей, но мне как-то лень
        }
        return make_shared<DumpBlock>(input.front());
    };
    void execute(conveyor& curStage) override;
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
};
