//#include "GameCore.h"
//
//void GameCore::runTournament(int cntOfRounds, const string &firstPlayerType, const string &secondPlayerType) {
//    shared_ptr<IGamer> player0 = gamersType[firstPlayerType]->prepareForBattle();
//    shared_ptr<IGamer> player1 = gamersType[secondPlayerType]->prepareForBattle();
//    Judge judge;
//    uint16_t player0_stat = 0;
//    uint16_t player1_stat = 0;
//    for (int i = 0; i < cntOfRounds; ++i){
//        switch (judge.playRound(player0, player1)) {
//            case 0: ++player0_stat;
//            case 1: ++player1_stat;
//        }
////        player0.clear(); //TODO: расширить для > 1 раунда, те реализовать очистку геймеров чи шо
////        player1.clear();
//    }
//}
//
//void GameCore::run(int cntOfRounds, const string &firstPlayerType, const string &secondPlayerType) {
//
//}
