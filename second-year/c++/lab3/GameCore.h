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
    char curStepResult = 0;
    IGamer* makeGamer(const string& gamerType);
    char processingShot(const square& curShot, IGamer* receivingPart);
    void processingNextStep(IGamer* player0, IGamer* player1);
    char playRound(IGamer* player0, IGamer* player1);
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
