#pragma once

#include <iostream>
#include <list>
#include <vector>
#include "IGamer.h"

class ConsoleGamer: public IGamer {
private:
    char *enemyField;
    std::list<Ship> fleetList;
    Ship **fleetMap;
    uint8_t fleetSize = 0;
public:
    ConsoleGamer(){
        enemyField = (char*)calloc(10 * 10, sizeof(char));
        fleetMap = (Ship**)calloc(10 * 10, sizeof(nullptr));
        fleetSize = 10;
    }
    ~ConsoleGamer() override {
        free(fleetMap);
        free(enemyField);
        fleetList.clear();
    }
    void setFleet() override;//TODO: это не так должно работать
    std::shared_ptr<IGamer> prepareForBattle() override;
    char takeHit(const square &curShot) override;
    square makeShot() override;//TODO
    void processingShotResult(const square &curShot, char result) override;
    uint8_t getCurFleetSize() override;
};
