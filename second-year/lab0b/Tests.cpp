#include <gtest/gtest.h>
#include "FileReader.h"
#include "WordStatService.h"
#include "StringParser.h"
#include <iostream>
#include <list>
#include <vector>

TEST(FileReader, getNextLine){
    FileReader fr("C:\\Users\\user\\CLionProjects\\lab0b\\testFiles\\testInputFile.txt");
    FileReader emptyFR("C:\\Users\\user\\CLionProjects\\lab0b\\testFiles\\emptyInputFile.txt");
    string nextLine;
    ASSERT_TRUE(fr.getNextLine(nextLine));
    ASSERT_EQ(nextLine, "f1rst_String1");
    ASSERT_FALSE(emptyFR.getNextLine(nextLine));
    ASSERT_TRUE(nextLine.empty());
}

TEST(WordStatService, getStatistic){
    WordStatService wordStatService;
    wordStatService.putWordInSet("я русский");
    wordStatService.putWordInSet("я русский");
    wordStatService.putWordInSet("one");
    wordStatService.putWordInSet("two");
    wordStatService.putWordInSet("two");
    wordStatService.putWordInSet("three");
    wordStatService.putWordInSet("three");
    wordStatService.putWordInSet("three");
    vector<pair<string, pair<unsigned int, double>>> expectedStat = {{"three", {3, 37.5}},
                                                                     {"two", {2, 25}},
                                                                     {"я русский", {2, 25}},
                                                                     {"one", {1, 12.5}}};
    vector<pair<string, pair<unsigned int, double>>> receivedStat = wordStatService.getStatistic();
    ASSERT_EQ(expectedStat, receivedStat);
}

TEST(StringParser, parseString){
    StringParser fileParser;
    list<string> expectedList = {"123", "qqwe1"};
    ASSERT_EQ(fileParser.parseString("123&_ qqwe1{(-_-)/"), expectedList);
    expectedList = {"AaAaQtyqui1231ui", "qweqweqQ"};
    ASSERT_EQ(fileParser.parseString("AaAaQtyqui1231ui^qweqweqQ"), expectedList);
    expectedList.resize(0);
    ASSERT_EQ(fileParser.parseString("!@#$%^&*(*&^%$#{:}><>?~`:;}+=-_-"), expectedList);
}
