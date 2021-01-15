#pragma once

#include "IGameView.h"

class ConsoleView: public IGameView {
public:
    ~ConsoleView() override = default;
    inline void clearWindow() override;
    void createWindow(int width, int height) override;
};
