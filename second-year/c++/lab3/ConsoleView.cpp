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
    std::cout << "   0 1 2 3 4 5 6 7 8 9" << std::endl;
    for (int y = 0; y < 10; ++y){
        printf(" %c ", 'A' + y);
        for (int x = 0; x < 10; ++x){
            if (fleetMap[y * 10 + x] == nullptr){
                printf("~ ");
            } else {
                printf("# ");
            }
        }
        printf("\n");
    }
    printf("\n");
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

void ConsoleView::printCurGamerFleetMap(Ship **curGamerFleetMap, char *viewFromOtherSide) {
    std::cout << "   0 1 2 3 4 5 6 7 8 9" << std::endl;
    for(int y = 0; y < 10; ++y){
        printf(" %c ", 'A' + y);
        for (int x = 0; x < 10; ++x){
            if (curGamerFleetMap[y * 10 + x] != nullptr){
                if (viewFromOtherSide[y * 10 + x] >= 1){ // 1 или 2
                    printf("X ");
                } else {
                    printf("# ");
                }
            } else {
                if (viewFromOtherSide[y * 10 + x] == 0){
                    printf("~ ");
                } else if (viewFromOtherSide[y * 10 + x] == -1){
                    printf("* ");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

void ConsoleView::printEnemyField(char *enemyField) {
    clearWindow();
    std::cout << "   0 1 2 3 4 5 6 7 8 9" << std::endl;
    for (int y = 0; y < 10; ++y){
        printf(" %c ", 'A' + y);
        for (int x = 0; x < 10; ++x){
            if (enemyField[y * 10 + x] == 0){
                printf("~ ");
            } else if (enemyField[y * 10 + x] >= 1){ // 1 или 2
                printf("X ");
            } else if(enemyField[y * 10 + x] == -1){
                printf("* ");
            }
        }
        printf("\n");
    }
    printf("\n");
}
