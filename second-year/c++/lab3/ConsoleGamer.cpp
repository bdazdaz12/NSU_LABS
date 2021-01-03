#include "ConsoleGamer.h"

void ConsoleGamer::setFleet() {
//    fleetList.emplace_back(2);
//    fleetMap[1 * 10 + 1] = &fleetList.front();
}

std::shared_ptr<IGamer> ConsoleGamer::prepareForBattle() {
    setFleet();
    return std::shared_ptr<ConsoleGamer>();
}

char ConsoleGamer::takeHit(const square &curShot) {
    char result = -1; //пустая клетка
    if (fleetMap[curShot.y * 10 + curShot.x]){
        result = fleetMap[curShot.y * 10 + curShot.x]->takeHit() ? 2 : 1; // 1 - попал, 2 - убил
    }
    if (result == 2){
        fleetSize--;
    }
    return result;
}

square ConsoleGamer::makeShot() {
    std::cout << "";
    ///считываем координаты выстрела из консоли и возвращаем
    return {1, 2};
}

uint8_t ConsoleGamer::getCurFleetSize() {
    return fleetSize;
}

void ConsoleGamer::processingShotResult(const square &curShot, char result) {
    enemyField[curShot.y * 10 + curShot.x] = result;
    if (result == 2){

    }
}


