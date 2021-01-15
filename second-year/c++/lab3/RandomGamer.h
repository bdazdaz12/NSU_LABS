#pragma once

#include "IGamer.h"
#include <list>

class RandomGamer: public IGamer {
private:
    char *enemyField;
    std::list<Ship> fleetList;
    Ship **fleetMap;
    std::uint8_t fleetSize = 0;
    bool isGoodLocate(char x, char y, bool horizontal, uint8_t shipLength) const;
    void setShip(char x, char y, bool horizontal, char shipLength, Ship *ship) const;
    inline void surroundShip(const Ship& destroyedShip);
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
    void setFleet() override;
    void prepareForBattle() override;
    char takeHit(const square &curShot) override;
    square makeShot() override;
    void processShotResult(const square &curShot, char result) override;
    void processDestruction(const Ship &destroyedShip) override;
    const Ship & getShipByCoord(const square &square) override;
    uint8_t getCurFleetSize() override;
};
