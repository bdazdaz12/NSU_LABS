#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include "ConsoleGamer.h"
#include "RandomGamer.h"
#include "OptimalGamer.h"

//TEST(PlayerLogic, OptimalSetFleet) {
////    std::shared_ptr<IGamer> gamer = RandomGamer().prepareForBattle();
//    OptimalGamer optimalGamer;
//    optimalGamer.prepareForBattle();
//    optimalGamer.processShotResult({4, 4}, 1);
//    optimalGamer.processShotResult({5, 4}, 1);
//    optimalGamer.processShotResult({6, 4}, 2);
//    optimalGamer.processDestruction(Ship(4, 4, true, 3, 3));
//    for (int i = 0; i < 10; ++i){
//        for (int j = 0; j < 10; ++j){
//            char ans;
//            if (optimalGamer.enemyField[i * 10 + j] == -1){
//                ans = '*';
//            } else if (optimalGamer.enemyField[i * 10 + j] >= 1){
//                ans = 'X';
//            } else {
//                ans = '.';
//            }
//            std::cout << ans << " ";
//        }
//        std::cout << '\n';
//    }
//}
