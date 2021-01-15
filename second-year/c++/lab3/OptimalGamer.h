#pragma once

#include <list>
#include "IGamer.h"

enum Direction: uint8_t {
    Up = 1,
    Down = 4,
    Right = 2,
    Left = 3,
    Unknown = 0
};

class OptimalGamer: public IGamer {
private:
    char *enemyField;
    std::list<Ship> fleetList;
    Ship **fleetMap;
    std::uint8_t fleetSize = 0;
    bool isTrueDirectionDefined = false; //samurai only has a way
    bool haveShipToBurn = false; //wake the fuk up, samurai
    square shipToBurn{};
    uint8_t direction = Unknown;
    bool isGoodLocate(char x, char y, bool horizontal, uint8_t shipLength) const;
    void setShip(char x, char y, bool horizontal, char shipLength, Ship *ship) const;
    inline void surroundShip(const Ship& destroyedShip);
    bool canMoveInDirection(char x, char y, uint8_t direction) const;
public:
    OptimalGamer(){
        enemyField = (char*)calloc(10 * 10, sizeof(char));
        fleetMap = (Ship**)calloc(10 * 10, sizeof(nullptr));
    }
    ~OptimalGamer() override {
        free(fleetMap);
        free(enemyField);
        fleetList.clear();
    };
    void setFleet() override;
    void prepareForBattle() override;
    void prepareForNewBattle() override;
    char takeHit(const square &curShot) override;
    square makeShot() override;//TODO: отдебажить
    void processShotResult(const square &curShot, char result) override;
    void processDestruction(const Ship &destroyedShip) override;
    const Ship & getShipByCoord(const square &square) override;
    uint8_t getCurFleetSize() override;
};
