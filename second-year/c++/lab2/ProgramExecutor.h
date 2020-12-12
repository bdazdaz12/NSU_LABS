#pragma once

#include "IWorker.h"
#include <map>
#include <memory>
#include <list>


class ProgramExecutor {
private:
    struct blueprint{
        list<uint32_t>& queue;
        map<uint32_t, shared_ptr<IWorker>>& blocks;
    };
public:
    ProgramExecutor() = default;
    ~ProgramExecutor() = default;
    void executeWorkflow(blueprint& workflow);
};
