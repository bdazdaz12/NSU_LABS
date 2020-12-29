#include "ProgramParser.h"


inline bool isFileExists(const string& fileName){
    ifstream inputFile(fileName);
    bool isExists = inputFile.good();
    inputFile.close();
    return isExists;
}

inline void errorHandler(const string&& message){
    throw invalid_argument(message);
}

list<string> parseString(const string &inputStr){
    list<string> wordsInStr;
    string curWord;
    for(const char& i : inputStr){
        if(i != ' '){
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

shared_ptr<IWorker> ProgramParser::parseBlock(ifstream& workflowFile) {
    string nextWord;
    workflowFile >> nextWord;
    if (nextWord != "="){
        errorHandler("Haven't '=' at block part!");
    }
    workflowFile >> nextWord;
    string args;
    getline(workflowFile, args);
    return blockMap[nextWord]->initialize(parseString(args));
}

map<int, shared_ptr<IWorker>> ProgramParser::parseBlockPart(ifstream& workflowFile){
    if (workflowFile.eof()) {
        errorHandler("EOF file!");
    }
    map<int, shared_ptr<IWorker>> blocks;
    string nextLine;
    workflowFile >> nextLine;
    if (nextLine != "desc") {
        errorHandler("Haven't 'desc' at block part!");
    }
    uint32_t nextIdx;
    workflowFile >> nextLine;
    while (nextLine != "csed") {
        try {
            nextIdx = stoi(nextLine);
            if (blocks.count(nextIdx)) {
                errorHandler("Index " + to_string(nextIdx) + " already exists!");
            } else {
                blocks[nextIdx] = parseBlock(workflowFile);
            }
            if (workflowFile.eof()) {
                errorHandler("Don't have 'csed' at end of blocks part!");
            }
            workflowFile >> nextLine;
        } catch (exception &e) {
            throw invalid_argument(e.what());
        }
    }
    return blocks;
}

list<int> parseQueuePart(ifstream& workflowFile){
    if (workflowFile.eof()) {
        errorHandler("EOF file!");
    }
    string nextWord;
    uint32_t nextIdx;
    list<int> queue;
    while (!workflowFile.eof()) {
        try {
            workflowFile >> nextWord;
            nextIdx = stoi(nextWord);
            queue.push_back(nextIdx);
            if (workflowFile.eof()) {
                break;
            }
            workflowFile >> nextWord;
            if (nextWord != "->") {
                errorHandler("Unknown symbol at queue part: " + nextWord);
            }
        } catch (exception &e) {
            throw invalid_argument(e.what());
        }
    }
    return queue;
}

Blueprint::blueprint ProgramParser::parseProgram(const string &workflow, const string &inputFile,
                                                 const string &outputFile){
    if (!isFileExists(workflow)){
        errorHandler("Wrong input from prompt!");
    }
    ifstream workflowFile(workflow);
    map<int, shared_ptr<IWorker>> &&blocks = parseBlockPart(workflowFile);
    list<int> &&queue = parseQueuePart(workflowFile);
    if(!inputFile.empty()){
        queue.push_front(-1);
        blocks[-1] = blockMap["readfile"]->initialize({inputFile});
    }
    if(!outputFile.empty()){
        queue.push_back(-2);
        blocks[-2] = blockMap["writefile"]->initialize({outputFile});
    }
    return {queue, blocks};
}
