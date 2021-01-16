#include "GameCore.h"

IGamer* GameCore::makeGamer(const string &gamerType) {
    if(gamerType == "console"){
        return new ConsoleGamer();
    } else if (gamerType == "random"){
        return new RandomGamer();
    } else if (gamerType == "optimal"){
        return new OptimalGamer();
    } else {
        return nullptr;
    }
}

void GameCore::requestGamerSetFleet(const string &playerType, IGamer *player) {
    Ship **fleetSetMap;
    if(playerType == "console"){
        while(player->getCurFleetSize() != 10){
            gameView->showFleetLocationMenu(player->getYourFleetMap());
            fleetSetMap = player->setFleet();
            if(fleetSetMap == nullptr){
                break; //он вбрал рандомную расстановку
            }
        }
        gameView->clearWindow();
    } else {
        player->setFleet();
    }
}

void GameCore::run(int cntOfRounds, const string &firstPlayerType, const string &secondPlayerType) {
    gameView->clearWindow();
    gameView->paintWindow();
    IGamer* player0 = makeGamer(firstPlayerType);
    IGamer* player1 = makeGamer(secondPlayerType);
    requestGamerSetFleet(firstPlayerType, player0);
    requestGamerSetFleet(secondPlayerType, player1);

    int player0_stat = 0;
    int player1_stat = 0;
    for (int i = 0; i < cntOfRounds; ++i){
        playRound(player0, player1) == 0? ++player0_stat : ++player1_stat;
        if (cntOfRounds - player0_stat - player1_stat >= 1) { // чтобы не запускать расстановку кораблей в конце
            player0->clear();
            player1->clear();
            requestGamerSetFleet(firstPlayerType, player0);
            requestGamerSetFleet(secondPlayerType, player1);
        }
    }
    delete player0;
    delete player1;
}

char GameCore::playRound(IGamer *player0, IGamer *player1) {
    curPlayer = player0;
    receivingPart = player1;
    curStepResult = -1;
    while(player0->getCurFleetSize() && player1->getCurFleetSize()){
        gameView->printEnemyField(curPlayer->getEnemyField());
        gameView->printCurGamerFleetMap(curPlayer->getYourFleetMap(),
                                        receivingPart->getEnemyField());
        processingNextStep();
        if (curStepResult == -1){
            swap(curPlayer, receivingPart); //TODO: надеюсь тут не будет багов
        }
    }
    return player0->getCurFleetSize() != 0? 0 : 1;
}

char GameCore::processingShot(const square &curShot) {
    return receivingPart->takeHit(curShot);
}

void GameCore::processingNextStep() {
    square curShot = curPlayer->makeShot();
    curStepResult = processingShot(curShot);
    curPlayer->processShotResult(curShot, curStepResult);
    if (curStepResult == 2) {
        curPlayer->processDestruction(receivingPart->getShipByCoord(curShot));
    }
}
