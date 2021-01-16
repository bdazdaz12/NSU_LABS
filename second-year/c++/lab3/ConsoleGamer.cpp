#include "ConsoleGamer.h"


bool ConsoleGamer::isGoodLocate(char x, char y, bool horizontal, uint8_t shipLength) const {
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

void ConsoleGamer::setShip(char x, char y, bool horizontal, char shipLength, Ship *ship) const {
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

void ConsoleGamer::setRandomFleet(){
    fleetSize = 10;
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

Ship** ConsoleGamer::setFleet() {
    while (tolower(shipLocateMode, std::locale()) != 'y' && tolower(shipLocateMode, std::locale()) != 'n') {
        std::cout << "Do you want to set the ships by yourself? (y/n): ";
        std::cin >> shipLocateMode;
    }
    if (tolower(shipLocateMode, std::locale()) == 'n'){
        setRandomFleet();
        return nullptr; //TODO: если возвращается NULL, значит игрок выбрал RandomSET
    } else {
        char x = 0, y = 0;
        char horizontal = 0;
        int shipType = 0;
        while(fleetSize != 10){
            printf("Entre ship args: ");
            std::cin >> y >> x >> horizontal >> shipType;
            if(!(x >= '0' && x <= '9' && y >= 'A' && y <= 'J' && (horizontal == 'y' || horizontal == 'n')
                 && shipType >= boat && shipType <= battleship)){
                printf("Wrong input! Pls repeat\n");
                continue;
            }
            x -= '0';
            y -= 'A';
            switch (shipType) {
                case battleship: {
                    if (battleships == 1) {
                        std::cout << "Wrong ship type! You already have a battleship!" << std::endl;
                        continue;
                    }
                    break;
                }
                case cruiser:{
                    if (cruisers == 2) {
                        std::cout << "Wrong ship type! You already have a cruisers!" << std::endl;
                        continue;
                    }
                    break;
                }
                case destroyer:{
                    if (destroyers == 3) {
                        std::cout << "Wrong ship type! You already have a destroyers!" << std::endl;
                        continue;
                    }
                    break;
                }
                case boat:{
                    if (boats == 4) {
                        std::cout << "Wrong ship type! You already have a boats!" << std::endl;
                        continue;
                    }
                    break;
                }
                default:
                    break;
            }
            if (!isGoodLocate(x, y, horizontal == 'y', shipType)){
                std::cout << "You can't locate ship at this area!" << std::endl;
                continue;
            } else {
                fleetList.emplace_back(x, y, horizontal == 'y', shipType, shipType);
                setShip(x, y, horizontal == 'y', shipType, &fleetList.back());
                fleetSize++;
                switch (shipType){
                    case battleship: {
                        battleships++;
                        break;
                    }
                    case cruiser:{
                        cruisers++;
                        break;
                    }
                    case destroyer:{
                        destroyers++;
                        break;
                    }
                    case boat:{
                        boats++;
                        break;
                    }
                }
                return fleetMap;// TODO
            }
        }
        return nullptr;
    }
}

char ConsoleGamer::takeHit(const square &curShot) {
    char result = -1; //пустая клетка
    if (fleetMap[curShot.y * 10 + curShot.x]){
        result = fleetMap[curShot.y * 10 + curShot.x]->takeHit()? 2 : 1; // 1 - попал, 2 - убил
    }
    if (result == 2){
        fleetSize--;
    }
    return result;
}

bool ConsoleGamer::isCorrectSquare(char x, char y){
    return x >= 0 && x < 10 && y >= 0 && y < 10 && enemyField[y * 10 + x] == 0;
}

square ConsoleGamer::makeShot() {
    std::cout << "Enter the coordinates of the next square in format x(num = {0, ..., 9}) y(char = {A, ..., J})\n";
    std::cout << "Enter it here: "; //TODO
    char x = 28, y = 28;
    bool isCorrect = true;
    do {
        if (!isCorrect){
            std::cout << "Wrong square! Pls repeat: ";
        }
        std::cin >> x >> y;
        x -= '0';
        y -= 'A';
    } while(!(isCorrect = isCorrectSquare(x, y)));
    return {x, y};
}

uint8_t ConsoleGamer::getCurFleetSize() {
    return fleetSize;
}

void ConsoleGamer::processShotResult(const square &curShot, char result) {
    enemyField[curShot.y * 10 + curShot.x] = result;
}

inline void ConsoleGamer::surroundShip(const Ship& destroyedShip){
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

void ConsoleGamer::processDestruction(const Ship &destroyedShip) {
    surroundShip(destroyedShip);
}

const Ship &ConsoleGamer::getShipByCoord(const square &square) {
    return *fleetMap[square.y * 10 + square.x];
}

void ConsoleGamer::clear() {
    for (int i = 0; i < 10; ++i){
        for (int j = 0; j < 10; ++j){
           enemyField[i * 10 + j] = 0;
           fleetMap[i * 10 + j] = nullptr;
        }
    }
    fleetSize = 0;
    fleetList.clear();
    battleships = 0;
    cruisers = 0;
    destroyers = 0;
    boats = 0;
    shipLocateMode = 0;
}

char* ConsoleGamer::getEnemyField() {
    return enemyField;
}

Ship **ConsoleGamer::getYourFleetMap() {
    return fleetMap;
}
