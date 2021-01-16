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

void GameCore::run(int cntOfRounds, const string &firstPlayer, const string &secondPlayer) {
    firstPlayerType = firstPlayer;
    secondPlayerType = secondPlayer;
    gameView->clearWindow();
    gameView->paintWindow();
    IGamer* player0 = makeGamer(firstPlayer);
    IGamer* player1 = makeGamer(secondPlayer);
    requestGamerSetFleet(firstPlayer, player0);
    requestGamerSetFleet(secondPlayer, player1);

    int player0_stat = 0;
    int player1_stat = 0;
    for (int i = 0; i < cntOfRounds; ++i){
        if (playRound(player0, player1) == 0){
            player0_stat++;
            gameView->printMessage("\tIn this round won PLAYER 1");
        } else {
            player1_stat++;
            gameView->printMessage("\tIn this round won PLAYER 2");
        }
        gameView->printMessage("\tPress ENTER to continue");
        getchar();
        if (cntOfRounds - player0_stat - player1_stat >= 1) { // чтобы не запускать расстановку кораблей в конце
            gameView->clearWindow();
            player0->clear();
            player1->clear();
            requestGamerSetFleet(firstPlayer, player0);
            requestGamerSetFleet(secondPlayer, player1);
        }
    }
    gameView->clearWindow();
    if (player0_stat > player1_stat){
        gameView->printMessage("\tPLAYER 1 is absolute winner!!! Congratulate!!!");
    } else if (player1_stat > player0_stat){
        gameView->printMessage("\tPLAYER 2 is absolute winner!!! Congratulate!!!");
    } else {
        gameView->printMessage("\tDRAW in result! Congratulate both of you!");
    }
    gameView->printMessage("\tPress ENTER to continue");
    getchar();
    delete player0;
    delete player1;
}

void GameCore::HUM_vs_HUM_mode(IGamer *player0, IGamer *player1) {
    while (player0->getCurFleetSize() && player1->getCurFleetSize()) {
        gameView->clearWindow();
        gameView->printMessage("Your enemy");
        gameView->printMessage(curPlayer == player0? "GAMER 2:" : "GAMER 1:"); //вверху противник
        gameView->printEnemyField(curPlayer->getEnemyField());
        gameView->printMessage("You");
        gameView->printMessage(curPlayer == player0? "GAMER 1:" : "GAMER 2:"); //снизу вы
        gameView->printCurGamerFleetMap(curPlayer->getYourFleetMap(),
                                        receivingPart->getEnemyField());
        processingNextStep();
        if (curStepResult == -1) {
            swap(curPlayer, receivingPart);
            gameView->wait(0.25);
            gameView->clearWindow();
            gameView->printMessage("Sides will change in 4 seconds. Pls call your friend!");
            gameView->wait(4);
        }
    }
    gameView->clearWindow();
    gameView->printMessage("Round is over.");
    gameView->printMessage("GAMER 1 MAP:");
    gameView->printCurGamerFleetMap(player0->getYourFleetMap(),
                                    player1->getEnemyField());
    gameView->printMessage("GAMER 2 MAP:");
    gameView->printCurGamerFleetMap(player1->getYourFleetMap(),
                                    player0->getEnemyField());
}

void GameCore::HUM_vs_PC_mode(IGamer *player0, IGamer *player1) {
    IGamer* human = firstPlayerType == "console"? player0 : player1;
    IGamer* PC = human == player0? player1 : player0;
    while (player0->getCurFleetSize() && player1->getCurFleetSize()) {
        gameView->clearWindow();
        gameView->printMessage("PC:");
        gameView->printEnemyField(human->getEnemyField());
        gameView->printMessage("YOUR FLEET:");
        gameView->printCurGamerFleetMap(human->getYourFleetMap(),
                                        PC->getEnemyField());
        processingNextStep();
        if (curStepResult == -1) {
            swap(curPlayer, receivingPart);
        }
    }
    gameView->clearWindow();
    gameView->printMessage("PC:");
    gameView->printCurGamerFleetMap(PC->getYourFleetMap(),
                                    human->getEnemyField());
    gameView->printMessage("YOUR FLEET:");
    gameView->printCurGamerFleetMap(human->getYourFleetMap(),
                                    PC->getEnemyField());
}

void GameCore::PC_vs_PC_mode(IGamer *player0, IGamer *player1) {//TODO delay будет
    while (player0->getCurFleetSize() && player1->getCurFleetSize()) {
        gameView->clearWindow();
        gameView->printMessage("PC1:");
        gameView->printCurGamerFleetMap(player0->getYourFleetMap(),
                                        player1->getEnemyField());
        gameView->printMessage("PC2:");
        gameView->printCurGamerFleetMap(player1->getYourFleetMap(),
                                        player0->getEnemyField());
        processingNextStep();
        if (curStepResult == -1) {
            swap(curPlayer, receivingPart);
        }
        gameView->wait(0.1); //TODO: задержка между ботами
    }
    gameView->clearWindow();
    gameView->printMessage("PC1:");
    gameView->printCurGamerFleetMap(player0->getYourFleetMap(),
                                    player1->getEnemyField());
    gameView->printMessage("PC2:");
    gameView->printCurGamerFleetMap(player1->getYourFleetMap(),
                                    player0->getEnemyField());
}

char GameCore::playRound(IGamer *player0, IGamer *player1) {
    curPlayer = player0;
    receivingPart = player1;
    curStepResult = -1;
    if (firstPlayerType != "console" && secondPlayerType != "console"){
        PC_vs_PC_mode(player0, player1);
    } else if (firstPlayerType == "console" && secondPlayerType == "console"){
        HUM_vs_HUM_mode(player0, player1);
    } else{
        HUM_vs_PC_mode(player0, player1);
    }
    gameView->wait(0.5);
    gameView->clearWindow();
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
