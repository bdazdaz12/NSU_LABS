#pragma once

#include "IToolkit.h"

class IValidator: protected IToolkit {
protected:
    virtual ~IValidator() = default;
public:
    virtual bool isValid(conveyor& curStage) = 0;
    virtual void errorHandler(string&& messages) = 0;
};