#pragma once

#include <string>
#include <vector>

class IToolkit {
protected:
    struct conveyor {
        std::vector<std::string>* input = nullptr;
        std::vector<std::string>* output = nullptr;
        int idx = -1;
        bool haveInput = false, haveOutput = false;
    };
};
