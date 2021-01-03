#pragma once

#include "BasicGameElements.h"
#include "IGamer.h"

using namespace std;

class Judge {
private:
    shared_ptr<IGamer> curPlayer;
    char curStepResult = 0;
    char processingShot(const square& curShot, const shared_ptr<IGamer>& receivingPart);
    void processingNextStep(const shared_ptr<IGamer>& player0, const shared_ptr<IGamer>& player1);
public:
    Judge() = default;
    ~Judge() = default;
    char playRound(const shared_ptr<IGamer>& player0, const shared_ptr<IGamer>& player1);
};
