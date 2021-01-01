#include "GameCore.h"

void GameCore::runTournament(int cntOfRounds, const string &firstPlayerType, const string &secondPlayerType) {
    shared_ptr<IGamer> player0 = gamersType[firstPlayerType]->prepareForBattle();
    shared_ptr<IGamer> player1 = gamersType[secondPlayerType]->prepareForBattle();
    Judge judge;
    for (int i = 0; i < cntOfRounds; ++i){
        judge.playRound(player0, player1); //TODO: расширить для > 1 раунда, те реализовать очистку геймеров чи шо
    }

}
