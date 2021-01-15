#pragma once

#include <string>
#include <map>
#include "IGamer.h"
#include "RandomGamer.h"
#include "ConsoleGamer.h"
#include "OptimalGamer.h"
#include "Judge.h"
#include "ConsoleView.h"
#include "IGameView.h"

using namespace std;

class GameCore {
private:
    IGameView* gameView = nullptr;
    IGamer* makeGamer(const string& gamerType);
public:
    explicit GameCore(const string& gameViewType){
        gameView = new ConsoleView;
        //TODO: add GUI
    }
    ~GameCore(){
        delete gameView;
    }
    void run(int cntOfRounds, const string& firstPlayerType, const string& secondPlayerType);
};
