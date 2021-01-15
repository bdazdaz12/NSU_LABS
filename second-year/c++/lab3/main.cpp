#include <iostream>
#include <gtest/gtest.h>
#include <ctime>
#include <memory>
#include "GameCore.h"
#include "optionparser.h"

using namespace std;


enum  optionIndex { UNKNOWN, HELP, PLUS };
const option::Descriptor usage[] ={
                {UNKNOWN, 0,"" , ""    ,option::Arg::None, "USAGE: example [options]\n\n"
                                                           "Options:" },
                {HELP,    0,"" , "help",option::Arg::None, "  --help  \tPrint usage and exit." },
                {PLUS,    0,"p", "plus",option::Arg::None, "  --plus, -p  \tIncrement count." },
                {UNKNOWN, 0,"" ,  ""   ,option::Arg::None, "\nExamples:\n"
                                                           "  example --unknown -- --this_is_no_option\n"
                                                           "  example -unk --plus -ppp file1 file2\n" },
                {0,0,0,0,0,0}
        };

int main(int argc, char** argv){
    argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
    option::Stats  stats(usage, argc, argv);
    option::Option options[stats.options_max], buffer[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);
    if (parse.error())
        return 1;
    if (options[HELP] || argc == 0) {
        option::printUsage(std::cout, usage);
        return 0;
    }
    std::cout << "--plus count: " <<
              options[PLUS].count() << "\n";
    for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
        std::cout << "Unknown option: " << opt->name << "\n";
    for (int i = 0; i < parse.nonOptionsCount(); ++i)
        std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";
}





//int main(int argc, char** argv){
//    srand(time(NULL));
//    GameCore gameCore;
//    gameCore.runTournament(2, "random", "random");
//}

//int main(int argc, char** argv) {
//    return RUN_ALL_TESTS();
//}
