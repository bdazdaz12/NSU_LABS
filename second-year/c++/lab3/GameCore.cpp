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

void GameCore::runTournament(int cntOfRounds, const string &firstPlayerType, const string &secondPlayerType) {
    gameView->createWindow(750, 330);
    IGamer* player0 = makeGamer(firstPlayerType);
    IGamer* player1 = makeGamer(secondPlayerType);
    player0->prepareForBattle();
    player1->prepareForBattle();
    Judge judge;
    uint16_t player0_stat = 0;
    uint16_t player1_stat = 0;
    for (int i = 0; i < cntOfRounds; ++i){
        if(judge.playRound(player0, player1) == 0){
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
    cout << player0_stat << " " << player1_stat;
}
