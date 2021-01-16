#pragma once

#include <string>
#include <map>
#include "IGamer.h"
#include "RandomGamer.h"
#include "ConsoleGamer.h"
#include "OptimalGamer.h"
#include "ConsoleView.h"
#include "IGameView.h"

using namespace std;

class GameCore {
private:
    IGameView* gameView = nullptr;
    IGamer* curPlayer = nullptr;
    IGamer* receivingPart = nullptr;
    char curStepResult = 0;
    IGamer* makeGamer(const string& gamerType);
    char processingShot(const square& curShot);
    void processingNextStep();
    char playRound(IGamer* player0, IGamer* player1);
    void requestGamerSetFleet(const string& playerType,IGamer* player);
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
