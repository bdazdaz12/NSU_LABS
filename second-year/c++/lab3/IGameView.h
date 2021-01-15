#pragma once

#include <iostream>
#include "Ship.h"

class IGameView{
public:
    virtual ~IGameView() = default;
    virtual void clearWindow() = 0;
    virtual void paintWindow() = 0;
    virtual void showFleetLocationMenu(Ship **fleetMap) = 0;
};