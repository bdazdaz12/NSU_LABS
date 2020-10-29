#include "StringParser.h"

list<string> StringParser::parseString(const string &inputStr){
    list<string> wordsInStr;
    string curWord;
    for(const char& i : inputStr){
        if(i >= 'a' && i <= 'z' ||  i >= '0' && i <= '9'
           || i >= 'A' && i <= 'Z'){
            curWord += i;
        } else if (!curWord.empty()) {
            wordsInStr.push_back(curWord);
            curWord.erase(0);
        }
    }
    if (!curWord.empty()){
        wordsInStr.push_back(curWord);
    }
    return wordsInStr;
}
