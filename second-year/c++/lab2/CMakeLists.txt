cmake_minimum_required(VERSION 3.17)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab2 main.cpp ProgramParser.cpp ProgramParser.h IValidator.h IWorker.h ReadFileBlock.cpp ReadFileBlock.h WriteFileBlock.cpp WriteFileBlock.h GrepBlock.cpp GrepBlock.h SortBlock.cpp SortBlock.h ReplaceBlock.cpp ReplaceBlock.h DumpBlock.cpp DumpBlock.h ProgramExecutor.cpp ProgramExecutor.h IToolkit.h)