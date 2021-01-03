#pragma once

#include <memory>
#include <string>
#include "Ship.h"
#include "BasicGameElements.h"

class IGamer {
public:
    virtual std::shared_ptr<IGamer> prepareForBattle() = 0;
    virtual ~IGamer() = default;
    virtual void setFleet() = 0;
    virtual char takeHit(const square& curShot) = 0;
    virtual square makeShot() = 0;
    virtual void processingShotResult(const square& curShot, char result) = 0;
    virtual uint8_t getCurFleetSize() = 0;
};
