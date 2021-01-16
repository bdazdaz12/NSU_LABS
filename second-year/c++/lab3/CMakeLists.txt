cmake_minimum_required(VERSION 3.17)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)
set(SRC main.cpp IGamer.h IGameView.h Ship.h ConsoleGamer.cpp ConsoleGamer.h BasicGameElements.h Tests.cpp RandomGamer.cpp RandomGamer.h GameCore.cpp GameCore.h ConsoleView.cpp ConsoleView.h OptimalGamer.cpp OptimalGamer.h optionparser.h)
include_directories(googletest/include)
add_executable(lab3 ${SRC})
target_link_libraries(lab3 gtest)