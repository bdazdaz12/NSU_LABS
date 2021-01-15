#include "RandomGamer.h"

bool RandomGamer::isGoodLocate(char x, char y, bool horizontal, uint8_t shipLength) const{
    if (horizontal){
        if (x < 0 || x + shipLength - 1 >= 10 || y < 0 || y >= 10) { //проверили можем ли разместить корабль
            return false;
        }
        for(int i = 0; i < shipLength; ++i) { //идем по X
            for(int j = -1; j <= 1; ++j) { // по Х
                for(int k = -1; k <= 1; ++k) { // по Y
                    if ((x + i + j >= 0 && x + i + j < 10) && (y + k >= 0 && y + k < 10)
                        && fleetMap[(y + k) * 10 + x + i + j] != nullptr){
                        return false;
                    }
                }
            }
        }
    } else {//по вертикали вниз от точки {x, y}
        if (x < 0 || x >= 10 || y < 0 || y + shipLength - 1 >= 10) {
            return false;
        }
        for(int i = 0; i < shipLength; ++i){ //идем по Y
            for(int j = -1; j <= 1; ++j) { // по Y
                for(int k = -1; k <= 1; ++k) { // по X
                    if ((y + i + j >= 0 && y + i + j < 10) && (x + k >= 0 && x + k < 10)
                        && fleetMap[(y + i + j) * 10 + x + k] != nullptr){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void RandomGamer::setShip(char x, char y, bool horizontal, char shipLength, Ship *ship) const {
    if (horizontal){
        for (int i = 0; i < shipLength; ++i) {
            fleetMap[y * 10 + x + i] = ship;
        }
    } else {
        for (int i = 0; i < shipLength; ++i) {
            fleetMap[(y + i) * 10 + x] = ship;
        }
    }
}

void RandomGamer::setFleet() {
    bool horizontal;
    char x, y;
    do {
        horizontal = rand() % 2 != 0;
        x = rand() % 10;
        y = rand() % 10;
    } while (!isGoodLocate(x, y, horizontal, battleship));
    fleetList.emplace_back(x, y, horizontal, battleship, 4); //чтобы не ссылаться на мусор, который пропадет при выходе
    setShip(x, y, horizontal, battleship, &fleetList.back());

    for (int i = 0; i < 2; ++i){
        do {
            horizontal = rand() % 2 != 0;
            x = rand() % 10;
            y = rand() % 10;
        } while (!isGoodLocate(x, y, horizontal, cruiser));
        fleetList.emplace_back(x, y, horizontal, cruiser, 3);
        setShip(x, y, horizontal, cruiser, &fleetList.back());
    }

    for (int i = 0; i < 3; ++i){
        do {
            horizontal = rand() % 2 != 0;
            x = rand() % 10;
            y = rand() % 10;
        } while (!isGoodLocate(x, y, horizontal, destroyer));
        fleetList.emplace_back(x, y, horizontal, destroyer, 2);
        setShip(x, y, horizontal, destroyer, &fleetList.back());
    }

    for (int i = 0; i < 4; ++i){
        do {
            horizontal = rand() % 2 != 0;
            x = rand() % 10;
            y = rand() % 10;
        } while (!isGoodLocate(x, y, horizontal, boat));
        fleetList.emplace_back(x, y, horizontal, boat, 1);
        setShip(x, y, horizontal, boat, &fleetList.back());
    }
}

char RandomGamer::takeHit(const square &curShot) {
    char result = -1; //пустая клетка
    if (fleetMap[curShot.y * 10 + curShot.x]){
        result = fleetMap[curShot.y * 10 + curShot.x]->takeHit()? 2 : 1; // 1 - попал, 2 - убил
    }
    if (result == 2){
        fleetSize--;
    }
    return result;
}

square RandomGamer::makeShot() {
    char x, y;
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (enemyField[y * 10 + x] != 0);
    return {x, y};
}

void RandomGamer::prepareForBattle(IGameView* gameView) {
    setFleet();
}

uint8_t RandomGamer::getCurFleetSize() {
    return fleetSize;
}

void RandomGamer::processShotResult(const square &curShot, char result) {
    enemyField[curShot.y * 10 + curShot.x] = result;
}

inline void RandomGamer::surroundShip(const Ship& destroyedShip){
    char x = destroyedShip.getX();
    char y = destroyedShip.getY();
    for (int i = 0; i < destroyedShip.getLength(); ++i){
        if (destroyedShip.getHorizontal()) { ///идем по X
            for (int j = -1; j <= 1; ++j) { // по Х
                for (int k = -1; k <= 1; ++k) { // по Y
                    if ((x + i + j >= 0 && x + i + j < 10) && (y + k >= 0 && y + k < 10)
                        && enemyField[(y + k) * 10 + x + i + j] == 0) {
                        enemyField[(y + k) * 10 + x + i + j] = -1; //означиваем ореол промахами
                    }
                }
            }
        } else { ///идем по Y
            for (int j = -1; j <= 1; ++j) { // по Y
                for (int k = -1; k <= 1; ++k) { // по X
                    if ((y + i + j >= 0 && y + i + j < 10) && (x + k >= 0 && x + k < 10)
                        && enemyField[(y + i + j) * 10 + x + k] == 0) {
                        enemyField[(y + i + j) * 10 + x + k] = -1;
                    }
                }
            }
        }
    }
}

void RandomGamer::processDestruction(const Ship &destroyedShip) {
    surroundShip(destroyedShip);
}

const Ship &RandomGamer::getShipByCoord(const square &square) {
    return *fleetMap[square.y * 10 + square.x];
}

void RandomGamer::prepareForNewBattle() {
    for (int i = 0; i < 10; ++i){
        for (int j = 0; j < 10; ++j){
            enemyField[i * 10 + j] = 0;
            fleetMap[i * 10 + j] = nullptr;
        }
    }
    fleetSize = 10;
    fleetList.clear();
    setFleet();
}
