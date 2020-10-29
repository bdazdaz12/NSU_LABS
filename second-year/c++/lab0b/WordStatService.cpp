#include <algorithm>
#include "WordStatService.h"

void WordStatService::putWordInSet(const string &word) {
    wordCounter[word]++;
}

unsigned int WordStatService::getAllFreqForCurStat() {
    unsigned int allFreq = 0;
    for (const auto& pair: wordCounter){
        allFreq += pair.second;
    }
    return allFreq;
}

vector<pair<string, pair<unsigned int, double>>> WordStatService::getStatistic(){
    unsigned int allFreq = getAllFreqForCurStat();
    vector<pair<string, pair<unsigned int, double>>> statistic; //TODO: почему то ломается при иниц (wordCounter.size())
    for(const auto& pair: wordCounter){
        std::pair<string, std::pair<unsigned int, double>> curPair =
                {pair.first, {pair.second,  (double)pair.second * 100 / allFreq}};
        statistic.emplace_back(curPair);
    }
    sort(statistic.begin(), statistic.end(), [](pair<string, pair<unsigned int, double>>& a,
            pair<string, pair<unsigned int, double>>& b)
                            {return a.second.first > b.second.first;});
    return statistic;
}