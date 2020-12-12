#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include "IValidator.h"
#include "IWorker.h"
#include "ProgramExecutor.h"
#include "DumpBlock.h"
#include "GrepBlock.h"
#include "ReadFileBlock.h"
#include "ReplaceBlock.h"
#include "SortBlock.h"
#include "WriteFileBlock.h"

using namespace std;

class ProgramParser {
public:
    static void parseProgram(const string& file, const string& inputFile, const string& outputFile);//TODO:
};
