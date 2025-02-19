#pragma once

#include "IWorker.h"
#include "IValidator.h"
#include <fstream>

class ReadFileBlock: public IWorker, IValidator {
private:
    string filename;
    ifstream inputFile;
public:
    ReadFileBlock() = default;
    explicit ReadFileBlock(const string& filename){
        this->filename = filename;
        inputFile.open(filename);
    };
    ~ReadFileBlock() override {
        inputFile.close();
    };
    shared_ptr<IWorker> initialize(const list<string>& input) override{
        if (input.empty()){
            throw invalid_argument("Not enough input in 'readfile' block!");
        }
        return make_shared<ReadFileBlock>(input.front());
    };
    string isValid(const conveyor& curStage) override;
    void errorHandler(string&& messages) override;
    void execute(conveyor& curStage) override;
};
