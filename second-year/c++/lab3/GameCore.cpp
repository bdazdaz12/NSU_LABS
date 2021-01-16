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

void GameCore::run(int cntOfRounds, const string &firstPlayerType, const string &secondPlayerType) {
    gameView->clearWindow();
    gameView->paintWindow();
    IGamer* player0 = makeGamer(firstPlayerType);
    IGamer* player1 = makeGamer(secondPlayerType);
    player0->prepareForBattle(gameView);
    player1->prepareForBattle(gameView);

    uint16_t player0_stat = 0;
    uint16_t player1_stat = 0;
    for (int i = 0; i < cntOfRounds; ++i){
        if(playRound(player0, player1) == 0){
            ++player0_stat;
        } else {
            ++player1_stat;
        }
        if (cntOfRounds > 1) {
            player0->prepareForNewBattle();
            player1->prepareForNewBattle();
        }
    }
    delete player0;
    delete player1;
}

char GameCore::playRound(IGamer *player0, IGamer *player1) {
    curPlayer = player0;
    curStepResult = -1;
    while(player0->getCurFleetSize() && player1->getCurFleetSize()){
        processingNextStep(player0, player1);
        if (curStepResult == -1){
            printf("change side\n");
            curPlayer = curPlayer == player0? player1 : player0;
        }
    }
    printf("%d \n", player0->getCurFleetSize());
    printf("%d \n", player1->getCurFleetSize());
    return player0->getCurFleetSize() != 0? 0 : 1;
}

char GameCore::processingShot(const square &curShot, IGamer *receivingPart) {
    return receivingPart->takeHit(curShot);
}

void GameCore::processingNextStep(IGamer *player0, IGamer *player1) {
    IGamer* receivingPart = curPlayer == player0? player1 : player0; ///терпила
    square curShot = curPlayer->makeShot();
    printf("cur shot: %d %d \n", curShot.x, curShot.y);
    curStepResult = processingShot(curShot, receivingPart);
    curPlayer->processShotResult(curShot, curStepResult);
    if (curStepResult == 2) {
        curPlayer->processDestruction(receivingPart->getShipByCoord(curShot));
    }
}
