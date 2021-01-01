#include "RandomGamer.h"

void RandomGamer::setFleet() {

}

char RandomGamer::takeHit(const square &curShot) {
    char result = -1; //пустая клетка
    if (fleetMap[curShot.x * 10 + curShot.y]){
        result = fleetMap[curShot.x * 10 + curShot.y]->takeHit()? 2 : 1; // 1 - попал, 2 - убил
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
    } while (!enemyField[x * 10 + y]);
    return {x, y};
}

std::shared_ptr<IGamer> RandomGamer::prepareForBattle() {
    setFleet();
    return std::shared_ptr<RandomGamer>();
}

uint8_t RandomGamer::getCurFleetSize() {
    return fleetSize;
}
