//#pragma once
//
//#include <string>
//#include <map>
//#include "IGamer.h"
//#include "RandomGamer.h"
//#include "ConsoleGamer.h"
//#include "Judge.h"
//
//using namespace std;
//
//class GameCore {
//private:
//    map<string, shared_ptr<IGamer>> gamersType = {{"console", make_shared<ConsoleGamer>()},
//                                                  {"random",  make_shared<RandomGamer>()}};
//public:
//    void run(int cntOfRounds, const string& firstPlayerType, const string& secondPlayerType);//TODO;
//    void runTournament(int cntOfRounds, const string& firstPlayerType, const string& secondPlayerType);
//};
