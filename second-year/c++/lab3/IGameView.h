#pragma once

#include <iostream>
#include <windows.h>

class IGameView{
public:
    virtual ~IGameView() = 0;
    virtual void clearWindow() = 0;
    virtual void createWindow(int width, int height) = 0;
};