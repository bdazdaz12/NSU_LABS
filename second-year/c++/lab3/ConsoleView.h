#pragma once

#include "IGameView.h"

class ConsoleView: public IGameView {
public:
    ~ConsoleView() override = default;
    inline void clearWindow() override;
    void paintWindow() override;
    void showFleetLocationMenu(Ship **fleetMap) override;
    void printFleetMap(Ship **fleetMap);
    void wait(float cntOfSeconds) override;
    void printCurGamerFleetMap(Ship **curGamerFleetMap, char *viewFromOtherSide) override;
    void printEnemyField(char *enemyField) override;
    void printMessage(const std::string &message) override;
};
