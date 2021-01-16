#pragma once

#include <memory>
#include <string>
#include "Ship.h"
#include "BasicGameElements.h"
#include "IGameView.h"

class IGamer {
public:
    virtual void clear() = 0;
    virtual ~IGamer() = default;
    virtual Ship** setFleet() = 0;
    virtual char takeHit(const square& curShot) = 0;
    virtual square makeShot() = 0;
    virtual void processShotResult(const square& curShot, char result) = 0;
    virtual void processDestruction(const Ship& destroyedShip) = 0;
    virtual const Ship& getShipByCoord(const square& square) = 0;
    virtual uint8_t getCurFleetSize() = 0;
    virtual char* getEnemyField() = 0;
    virtual Ship** getYourFleetMap() = 0;
};
