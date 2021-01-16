#include <iostream>
//#include <gtest/gtest.h>
#include <ctime>
#include <memory>
#include "GameCore.h"
#include "optionparser.h"

using namespace std;

struct Arg : public option::Arg {
    static void printError(const char *msg1, const option::Option &opt, const char *msg2) {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Unknown(const option::Option &option, bool msg) {
        if (msg) {
            printError("Unknown option '", option, "'\n");
        }
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Numeric(const option::Option &option, bool msg) {
        char *endptr = nullptr;
        if (option.arg != nullptr && strtol(option.arg, &endptr, 10)) {}; //goood
        if (endptr != option.arg && *endptr == 0) {
            return option::ARG_OK;
        }
        if (msg) {
            printError("Option '", option, "' requires a numeric argument\n");
        }
        return option::ARG_ILLEGAL;
    }
};

enum optionIndex {
    UNKNOWN, HELP, FIRST, SECOND, COUNT
};

const option::Descriptor usage[] = {
        {UNKNOWN, 0, "",  "",       Arg::Unknown,  "USAGE: example_arg [options]\n\n"
                                                   "Options:"},
        {HELP,    0, "h",  "help",   Arg::None,     "  \t-h --help  \tYou can choose the type of two players and the number of rounds in the series. Total player types 3:\n\t\t\tRandom - random gamer. Absolutely random AI;\n\t\t\tOptimal - it's most difficult AI (-_-);\n\t\t\tInteractive - it's you or your friend;\t"},
        {FIRST,   0, "f", "first",  Arg::Optional, "  -f[<arg>], \t--first[=<arg>]"
                                                   "  \tType of first player. Default is Random."},
        {SECOND,  0, "s", "second", Arg::Optional, "  -s[<arg>], \t--second[=<arg>]"
                                                   "  \tType of second player. Default is Random."},
        {COUNT,   0, "c", "count",  Arg::Numeric,  "  -c <num>, \t--count=<num>  \tNumber of rounds in a series. Default is 1."},
        {0,       0, nullptr,   nullptr,        nullptr,             nullptr}};


/* "-" - для коротких имен опций
 * "--" - для длинных имен опций
 * */

int main(int argc, char** argv){
    srand(time(nullptr));
    argc -= (argc > 0);
    argv += (argc > 0); // skip program name argv[0] if present
    option::Stats stats(usage, argc, argv);
    option::Option options[stats.options_max], buffer[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);

    if (parse.error()) {
        return 1;
    }
    if (options[HELP] || argc == 0) {
        option::printUsage(cout, usage);
        return 0;
    }
    if (options[UNKNOWN]) { //zx
        return 0;
    }
    if (parse.nonOptionsCount() > 0) { // если в консоли есть неизвестные аргументы
        for (int i = 0; i < parse.nonOptionsCount(); ++i) {
            printf("Unknown argument #%d is %s\n", i, parse.nonOption(i));
        }
        return 0;
    }

    int countOfRounds = 1;
    if (options[COUNT]) { //если в консоли есть аргумент типа COUNT
        countOfRounds = strtol(options[COUNT].arg, nullptr, 10);
        if (countOfRounds < 0) {
            printf("\n Error. Number of rounds must be positive!\n");
            return 0;
        } else if (countOfRounds == 0) {
            return 0;
        }
    }

    string firstPlayerType = "random";
    if (options[FIRST]) {
        if (((string) "Optimal") == options[FIRST].arg) {
            firstPlayerType = "optimal";
        } else if (((string) "Interactive") == options[FIRST].arg) {
            firstPlayerType = "console";
        } else if (((string) "Random") != options[FIRST].arg){
            printf("Unknown player type: %s!", options[FIRST].arg);
            return 1;
        }
    }
    string secondPlayerType = "random";
    if (options[SECOND]) {
        if (((string) "Optimal") == options[SECOND].arg) {
            secondPlayerType = "optimal";
        } else if (((string) "Interactive") == options[SECOND].arg) {
            secondPlayerType = "console";
        } else if (((string) "Random") != options[SECOND].arg){
            printf("Unknown player type: %s!", options[SECOND].arg);
            return 1;
        }
    }
    GameCore gameCore("console");
    gameCore.run(countOfRounds, firstPlayerType, secondPlayerType);
}


//int main(int argc, char** argv) {
//    return RUN_ALL_TESTS();
//}
