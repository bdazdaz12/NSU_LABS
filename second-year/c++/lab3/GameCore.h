#pragma once

#include <string>
#include <map>
#include "IGamer.h"
#include "RandomGamer.h"
#include "ConsoleGamer.h"
#include "OptimalGamer.h"
#include "Judge.h"

using namespace std;

class GameCore {
private:
    IGamer* makeGamer(const string& gamerType);
public:
    void runTournament(int cntOfRounds, const string& firstPlayerType, const string& secondPlayerType);
};
