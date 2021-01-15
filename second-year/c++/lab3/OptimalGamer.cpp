#include "OptimalGamer.h"

void OptimalGamer::setFleet() {
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

void OptimalGamer::prepareForBattle(IGameView* gameView) {
    fleetSize = 10;
    setFleet();
}

char OptimalGamer::takeHit(const square &curShot) {
    char result = -1; //пустая клетка
    if (fleetMap[curShot.y * 10 + curShot.x]){
        result = fleetMap[curShot.y * 10 + curShot.x]->takeHit()? 2 : 1; // 1 - попал, 2 - убил
    }
    if (result == 2){
        fleetSize--;
    }
    return result;
}

void reverseDirection(uint8_t& direction){
    direction = direction * 4 % 5; //thought about theory of groups
}

void tryDetermineDirection(uint8_t& direction){
    switch (direction) {
        case Unknown:
            direction = Right;
            break;
        case Right:
            direction = Left;
            break;
        case Left:
            direction = Up;
            break;
        case Up:
            direction = Down;
            break;
        case Down:
            direction = Unknown;
            break;
    }
}

void moveInDefinedDirection(char& x, char& y, uint8_t direction){
    switch (direction) { ///свичи - говно! У этой штуки ужасный синтаксис, какие breaks?! а фигурные скобки не работают(
        case Right:
            ++x;
            break;
        case Left:
            --x;
            break;
        case Up:
            --y;
            break;
        case Down:
            ++y;
            break;
    }
}

bool OptimalGamer::canMoveInDirection(char x, char y, uint8_t curDirection) const {
    if (curDirection == Unknown){
        return false;
    }
    moveInDefinedDirection(x, y, curDirection);
    if (x < 0 || x >= 10 || y < 0 || y >= 10 || enemyField[y * 10 + x] == -1){
        return false;
    }
    return true;
}

square OptimalGamer::makeShot() {
    char x, y;
    if (haveShipToBurn){
        x = shipToBurn.x;
        y = shipToBurn.y;
        if (isTrueDirectionDefined) {
            while (canMoveInDirection(x, y, direction) && enemyField[y * 10 + x] != 0) {
                //второе условие отвечает за "проход" старых удачных выстрелов
                moveInDefinedDirection(x, y, direction);
            }
            if (enemyField[y * 10 + x] != 0){
                reverseDirection(direction);
                while (canMoveInDirection(x, y, direction) && enemyField[y * 10 + x] != 0) {
                    moveInDefinedDirection(x, y, direction);
                }
            }
            //TODO: все это нужно отдебажить, писать это в 5 утра конечно без тестов очень продуктывно
        } else {
            do {
                tryDetermineDirection(direction);
            } while (!canMoveInDirection(x, y, direction)); //перв опред направления у точки, которую пока окруж ничего
            moveInDefinedDirection(x, y, direction);
            /* если выстрел будет неудачным, на следующей попытке определить направление мы просто не сможем пойти
             * по предыдущему направлению и попытаемя обработать три оставшихся
             * при этом при неправильном определении мы не сдвинимся из самой первой точки-попадания */
        }
    } else {
        do {
            x = rand() % 10;
            y = rand() % 10;
        } while (enemyField[y * 10 + x] != 0);
    }
    return {x, y};
}

void OptimalGamer::processShotResult(const square &curShot, char result) {
    if (result == 1){
        if (!haveShipToBurn) {
            isTrueDirectionDefined = false;
            haveShipToBurn = true;
            shipToBurn = curShot;
        } else {
            isTrueDirectionDefined = true;
            shipToBurn = curShot;
        }
    } else if (result == 2){
        haveShipToBurn = false;
    }
    enemyField[curShot.y * 10 + curShot.x] = result;
}

inline void OptimalGamer::surroundShip(const Ship& destroyedShip){
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

void OptimalGamer::processDestruction(const Ship &destroyedShip) {
    surroundShip(destroyedShip);
}

const Ship &OptimalGamer::getShipByCoord(const square &square) {
    return *fleetMap[square.y * 10 + square.x];
}

uint8_t OptimalGamer::getCurFleetSize() {
    return fleetSize;
}

bool OptimalGamer::isGoodLocate(char x, char y, bool horizontal, uint8_t shipLength) const {
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

void OptimalGamer::setShip(char x, char y, bool horizontal, char shipLength, Ship *ship) const {
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

void OptimalGamer::prepareForNewBattle() {
    for (int i = 0; i < 10; ++i){
        for (int j = 0; j < 10; ++j){
            enemyField[i * 10 + j] = 0;
            fleetMap[i * 10 + j] = nullptr;
        }
    }
    fleetSize = 10;
    fleetList.clear();
    isTrueDirectionDefined = false;
    haveShipToBurn = false;
    direction = Unknown;
    setFleet();
}
