#include <iostream>
#include <gtest/gtest.h>
#include <ctime>
#include <memory>
#include "OptimalGamer.h"
#include "RandomGamer.h"
#include "IGamer.h"
#include "Judge.h"

using namespace std;

int main(int argc, char** argv){
    srand(time(NULL));
    OptimalGamer optimalGamer0;
    OptimalGamer optimalGamer1;
    IGamer* player0 = &optimalGamer0;
    IGamer* player1 = &optimalGamer1;
    player0->prepareForBattle();
    player1->prepareForBattle();
    Judge judge;
    if (judge.playRound(player0, player1) == 0) {
        cout << "da";
    } else {
        cout << "net";
    }
}

//int main(int argc, char** argv) {
//    return RUN_ALL_TESTS();
//}
