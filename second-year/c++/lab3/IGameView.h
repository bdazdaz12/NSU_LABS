#pragma once

#include <iostream>
#include "Ship.h"
#include "IGamer.h"

class IGameView {
public:
    virtual ~IGameView() = default;
    virtual void clearWindow() = 0;
    virtual void paintWindow() = 0;
    virtual void showFleetLocationMenu(Ship **fleetMap) = 0;
    virtual void wait(int cntOfSeconds) = 0;
    virtual void printCurGamerFleetMap(Ship **curGamerFleetMap, char* viewFromOtherSide) = 0;
    virtual void printEnemyField(char* enemyField) = 0;
};