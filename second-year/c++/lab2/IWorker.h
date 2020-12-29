#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "IToolkit.h"
#include <list>

using namespace std;

class IWorker: protected IToolkit {
protected:
    virtual ~IWorker() = default;
public:
    virtual shared_ptr<IWorker> initialize(const list<string>& input) = 0;
    virtual void execute(conveyor& curStage) = 0;
};
