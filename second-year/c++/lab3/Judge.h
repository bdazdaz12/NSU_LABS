#pragma once

#include "BasicGameElements.h"
#include "IGamer.h"

using namespace std;

class Judge {
private:
    IGamer* curPlayer = nullptr;
    char curStepResult = 0;
    char processingShot(const square& curShot, IGamer* receivingPart);
    void processingNextStep(IGamer* player0, IGamer* player1);
public:
    Judge() = default;
    ~Judge() = default;
    char playRound(IGamer* player0, IGamer* player1);
};
