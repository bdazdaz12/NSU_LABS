#include "Judge.h"

char Judge::processingShot(const square &curShot, const shared_ptr<IGamer>& receivingPart) {
    return receivingPart->takeHit(curShot);
}

char Judge::playRound(const shared_ptr<IGamer>& player0, const shared_ptr<IGamer>& player1) {
    curPlayer = player0;//TODO возможно стоит изменить первого игрока("ходят белые")
    curStepResult = -1;
    while(player0->getCurFleetSize() && player1->getCurFleetSize()){
        processingNextStep(player0, player1);
        if (curStepResult == -1){
            curPlayer = curPlayer == player0? player1 : player0;
        }
    }
    return player0->getCurFleetSize()? 0 : 1;
}

void Judge::processingNextStep(const shared_ptr<IGamer>& player0, const shared_ptr<IGamer>& player1) {
    square curShot = curPlayer->makeShot();
    curStepResult = processingShot(curShot, curPlayer == player0 ? player1 : player0);
    curPlayer->processingShotResult(curShot, curStepResult);
}


