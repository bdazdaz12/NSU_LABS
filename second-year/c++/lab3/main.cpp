#include <iostream>
#include <gtest/gtest.h>
#include <ctime>
#include <memory>
#include "GameCore.h"

using namespace std;

int main(int argc, char** argv){
    srand(time(NULL));
    GameCore gameCore;
    gameCore.runTournament(2, "random", "random");
}

//int main(int argc, char** argv) {
//    return RUN_ALL_TESTS();
//}
