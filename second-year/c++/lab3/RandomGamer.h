#pragma once

#include "IGamer.h"
#include <list>

class RandomGamer: public IGamer {
private:
    char *enemyField;
    Ship **fleetMap;
    std::list<Ship> fleetList;
    std::uint8_t fleetSize = 0;
public:
    RandomGamer(){
        enemyField = (char*)calloc(10 * 10, sizeof(char));
        fleetMap = (Ship**)calloc(10 * 10, sizeof(nullptr));
        fleetSize = 10;
    }
    ~RandomGamer() override {
        free(fleetMap);
        free(enemyField);
        fleetList.clear();
    };
    void setFleet() override; //TODO
    std::shared_ptr<IGamer> prepareForBattle() override;
    char takeHit(const square &curShot) override;
    square makeShot() override; //TODO: сделать обновление враж карты(те запихнуть метод processAns or update в IGamer)
    uint8_t getCurFleetSize() override;
};
