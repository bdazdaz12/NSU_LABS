#include "ConsoleView.h"

inline void ConsoleView::clearWindow() {
    system("cls");
}

void ConsoleView::createWindow(int width, int height) {
    system("color F0");
    HWND hwnd;
    char Title[1024];
    GetConsoleTitle(Title, 1024);
    hwnd = FindWindow(nullptr, Title);
    MoveWindow(hwnd, 0, 0, width, height, TRUE);
}
