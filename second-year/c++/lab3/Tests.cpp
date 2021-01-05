#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include "ConsoleGamer.h"
#include "RandomGamer.h"
#include "OptimalGamer.h"

//TEST(PlayerLogic, OptimalSetFleet) {
//    std::shared_ptr<IGamer> gamer = RandomGamer().prepareForBattle();
//    OptimalGamer optimalGamer;
//    optimalGamer.prepareForBattle();
//    square a = {0, 0};
//    do{
//        a = optimalGamer.makeShot();
//    } while(a.x != 0 || a.y != 0);
//    optimalGamer.processShotResult(a, 1);
//    a = optimalGamer.makeShot();
//    std::cout << a.x << " " << a.y;


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
