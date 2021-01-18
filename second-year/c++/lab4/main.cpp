#include <iostream>
#include <tuple>
#include <fstream>
#include "CSVParser.h"

using namespace std;

int main() {
    tuple<int, int, string, int> a = {123, 321, "123", 4};
    ifstream ifstream1("in.csv");
    CSVParser<int, string>parser (ifstream1, 0, '\n', ',', '\"');
    for (const tuple<int, std::string>& rs : parser) { // считываем по 1 кортежу из строки
        std::cout << rs << std::endl;
    }
}
