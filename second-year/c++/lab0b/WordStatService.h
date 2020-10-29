#pragma once
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class WordStatService {
public:
    WordStatService() = default;
    void putWordInSet(const string& word);
    vector<pair<string, pair<unsigned int, double>>> getStatistic();
    unsigned int getAllFreqForCurStat();
private:
    map<string, unsigned int> wordCounter;
};
