#pragma once

#include "IGameView.h"

class ConsoleView: public IGameView {
public:
    inline void clearWindow() override;
};
