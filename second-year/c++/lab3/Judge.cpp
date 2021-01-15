#include "Judge.h"

char Judge::processingShot(const square &curShot, IGamer* receivingPart) {
    return receivingPart->takeHit(curShot);
}

char Judge::playRound(IGamer* player0, IGamer* player1) {
    curPlayer = player0;//TODO возможно стоит изменить первого игрока("ходят белые")
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

void Judge::processingNextStep(IGamer* player0, IGamer* player1) {
    IGamer* receivingPart = curPlayer == player0? player1 : player0; ///терпила
    square curShot = curPlayer->makeShot();
    printf("cur shot: %d %d \n", curShot.x, curShot.y);
    curStepResult = processingShot(curShot, receivingPart);
    curPlayer->processShotResult(curShot, curStepResult);
    if (curStepResult == 2) {
        curPlayer->processDestruction(receivingPart->getShipByCoord(curShot));
    }
}
