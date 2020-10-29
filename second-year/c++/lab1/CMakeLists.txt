cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)
set(SRC main.cpp Tests.cpp Trit.cpp Trit.h TritSet.cpp TritSet.h)
include_directories(googletest/include)
add_executable(lab1 ${SRC})
target_link_libraries(lab1 gtest)
