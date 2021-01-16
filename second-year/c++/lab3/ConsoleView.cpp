#include "ConsoleView.h"
#include <windows.h>

inline void ConsoleView::clearWindow() {
    system("cls");
}

void ConsoleView::paintWindow() {
    system("color F0");
}

void ConsoleView::printFleetMap(Ship **fleetMap) {
    clearWindow();
    std::cout << "  0 1 2 3 4 5 6 7 8 9" << std::endl;
    for (int i = 0; i < 10; ++i){
        printf("%c ", 'A' + i);
        for (int j = 0; j < 10; ++j){
            if (fleetMap[i * 10 + j] == nullptr){
                printf("~ ");
            } else {
                printf("# ");
            }
        }
        printf("\n");
    }
}

void ConsoleView::showFleetLocationMenu(Ship **fleetMap) {
    printFleetMap(fleetMap);
    std::cout << "Entre <y(char)> <x(int)> <horizontal(y/n)> <shipType(int)>  until you collect the field!" << std::endl;
    std::cout << "shipType:" << std::endl;
    std::cout << "\t1 - boat" << std::endl;
    std::cout << "\t2 - destroyer" << std::endl;
    std::cout << "\t3 - cruiser" << std::endl;
    std::cout << "\t4 - battleship" << std::endl << std::endl;
}

void ConsoleView::wait(int cntOfSeconds) {
    Sleep(cntOfSeconds * 1000);
}
