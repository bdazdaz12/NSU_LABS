#pragma once

#include <iostream>
#include <list>
#include <vector>
#include "IGamer.h"

class ConsoleGamer: public IGamer {
private:
    uint8_t battleships = 0;
    uint8_t cruisers = 0;
    uint8_t destroyers = 0;
    uint8_t boats = 0;
    char shipLocateMode = 0; // 'y' - by yourself, 'n' - randomShips

    char *enemyField;
    std::list<Ship> fleetList;
    Ship **fleetMap;
    uint8_t fleetSize = 0;

    bool isGoodLocate(char x, char y, bool horizontal, uint8_t shipLength) const;
    void setShip(char x, char y, bool horizontal, char shipLength, Ship *ship) const;
    inline void surroundShip(const Ship& destroyedShip);
    bool isCorrectSquare(char x, char y);
    void setRandomFleet();
public:
    ConsoleGamer(){
        enemyField = (char*)calloc(10 * 10, sizeof(char));
        fleetMap = (Ship**)calloc(10 * 10, sizeof(nullptr));
    }
    ~ConsoleGamer() override {
        free(fleetMap);
        free(enemyField);
        fleetList.clear();
    };
    Ship** setFleet() override;
    void clear() override;
    char takeHit(const square &curShot) override;
    square makeShot() override;
    void processShotResult(const square &curShot, char result) override;
    void processDestruction(const Ship &destroyedShip) override;
    const Ship & getShipByCoord(const square &square) override;
    uint8_t getCurFleetSize() override;
    char* getEnemyField() override;
    Ship ** getYourFleetMap() override;
};
