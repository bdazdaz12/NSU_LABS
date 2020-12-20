#pragma once

#include <iostream>
#include <vector>
#include "IToolkit.h"
//#include <list>

using namespace std;

class IWorker: protected IToolkit {
protected:
    virtual ~IWorker() = default;
public:
    virtual void execute(conveyor& curStage) = 0;
};
