cmake_minimum_required(VERSION 3.17)
project(lab0b)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)
set(SRC main.cpp FileReader.cpp FileReader.h WordStatService.cpp WordStatService.h StringParser.cpp StringParser.h ReportService.cpp ReportService.h Tests.cpp)
include_directories(googletest/include)
add_executable(lab0b ${SRC} FileReader.cpp)
target_link_libraries(lab0b gtest)
