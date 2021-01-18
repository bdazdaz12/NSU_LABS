#pragma once

#include <string>
#include <exception>
#include <sstream>
#include <istream>
#include <iterator>
#include <memory>
#include <cxxabi.h>
#include <vector>
#include "TuplePrintUtility.h"

template<typename T>
std::string GetTypeName() {
    int status;
    std::string tName = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tName.c_str(), nullptr, nullptr, &status);
    if (status == 0) {
        tName = demangled_name;
        std::free(demangled_name);
    }
    return tName;
}

template<typename ... Args>
class CSVReader {
public:
    explicit CSVReader(char lineDelimiter = '\n', char valueDelimiter = ',', char fieldDelimiter = '\"') :
            lineDelimiter(lineDelimiter),
            valueDelimiter(valueDelimiter),
            fieldDelimiter(fieldDelimiter) {}

    void readInTuple(std::istream &istream, std::tuple<Args...> &tuple) {
        readLine(istream, tuple, std::index_sequence_for<Args...>());
    }

    static bool canRead(std::istream &istream) {
        return istream.good() && istream.peek() != EOF;
    }

private:
    std::string nextLine;
    const char lineDelimiter;
    const char valueDelimiter;
    const char fieldDelimiter;
    size_t cntOfReadLines = 0;
    size_t curValuePos = 0;
    std::vector<std::string> lineFields;
    std::vector<size_t> delimitersPos;
    size_t curFieldIndex = 0;

    enum class CSVState {
        UnescapedField,
        EscapedField,
        EscapedEscape,
    };

    void SplitIntoStrings() {
        CSVState state = CSVState::UnescapedField;
        lineFields.clear();
        lineFields.emplace_back("");
        delimitersPos.clear();
        size_t i = 0; // index of the current field
        size_t curPos = 0;
        for (char c : nextLine) {
            switch (state) {
                case CSVState::UnescapedField:
                    if (c == valueDelimiter) {
                        lineFields.emplace_back("");
                        delimitersPos.push_back(curPos);
                        i++;
                    } else if (c == fieldDelimiter) {
                        state = CSVState::EscapedField;
                    } else {
                        lineFields[i].push_back(c);
                    }
                    break;
                case CSVState::EscapedField:
                    if (c == fieldDelimiter) {
                        state = CSVState::EscapedEscape;
                    } else {
                        lineFields[i].push_back(c);
                    }
                    break;
                case CSVState::EscapedEscape:
                    if (c == valueDelimiter) {
                        lineFields.emplace_back("");
                        delimitersPos.push_back(curPos);
                        i++;
                        state = CSVState::UnescapedField;
                    } else if (c == fieldDelimiter) {
                        lineFields[i].push_back(fieldDelimiter);
                        state = CSVState::EscapedField;
                    } else {
                        state = CSVState::UnescapedField;
                    }
                    break;
            }
            curPos++;
        }
    }

    template<size_t ... Idx>
    void readLine(std::istream &i, std::tuple<Args...> &tuple, std::index_sequence<Idx...>) {
        std::getline(i, nextLine, lineDelimiter);
        cntOfReadLines++;
        curValuePos = 0;
        curFieldIndex = 0;
        if (nextLine.empty()) {
            throw std::invalid_argument("Line is empty:\n\t Line = " + std::to_string(curValuePos + 1) +
                                                " Column = " + std::to_string(cntOfReadLines));
        }

        SplitIntoStrings();

        (..., (readValue(std::get<Idx>(tuple))));

        if (curFieldIndex != lineFields.size()) {
            std::stringstream stringstream;
            stringstream << "Unexpected value! " << lineFields[curFieldIndex];
            throw std::invalid_argument(stringstream.str() + "Line = " + std::to_string(cntOfReadLines) + " Column = " +
                                        std::to_string(curValuePos + 1) + "\n\t line: " + nextLine);
        }
    }
    template<typename T>
    void readValue(T &t) {
        std::string curField = lineFields[curFieldIndex];
        std::stringstream convert(curField);
        if ((convert >> t).fail() || !(convert >> std::ws).eof()) {
            std::stringstream stringstream;
            stringstream << "Couldn't parse value (expected type: " << GetTypeName<T>() << "): " << curField;
            throw std::invalid_argument(stringstream.str() + "Line = " + std::to_string(cntOfReadLines) + " Column = " +
                                        std::to_string(curValuePos + 1) + "\n\t line: " + nextLine);
        }
        curValuePos = delimitersPos[curFieldIndex] + 1;
        curFieldIndex++;
    }
};

template<typename ... Args>
class CSVIterator {
public:
    explicit CSVIterator(std::istream &basicIstream, char lineDelimiter = '\n', char valueDelimiter = ',',
                         char fieldDelimiter = '\"') :
            istream(basicIstream),
            canRead(basicIstream.good()),
            csvReader(lineDelimiter, valueDelimiter, fieldDelimiter) {++(*this); } //TODO: зачем

    explicit CSVIterator(std::istream &s, bool isEnd) : istream(s), canRead(!isEnd), csvReader() {}

    CSVIterator &operator++() {
        canRead = csvReader.canRead(istream);
        if (canRead) {
            csvReader.readInTuple(istream, nextTuple);
        }
        return *this;
    }

    CSVIterator operator++(int) {
        CSVIterator t(*this);
        ++(*this);
        return t;
    }

    const std::tuple<Args...> &operator*() const {
        return nextTuple;
    }

    const std::tuple<Args...> *operator->() const {
        return &nextTuple;
    }

    bool operator==(const CSVIterator &other) const {
        return this == &other || canRead == other.canRead;
    }

    bool operator!=(const CSVIterator &other) const {
        return !((*this) == other); //не трогай
    }

private:
    bool canRead = false;
    std::istream &istream;
    std::tuple<Args...> nextTuple;
    CSVReader<Args...> csvReader;
};

template<typename ... Args>
class CSVParser {
private:
    std::istream &inputStream;
    const char lineDelimiter = 0;
    const char valueDelimiter = 0;
    const char fieldDelimiter = 0;
    size_t skipLinesCount;

public:
    explicit CSVParser(std::istream &_istream,
                       size_t skipLinesCount = 0,
                       char lineDelimiter = '\n',
                       char valueDelimiter = ',',
                       char fieldDelimiter = '\"') :
            inputStream(_istream),
            lineDelimiter(lineDelimiter),
            valueDelimiter(valueDelimiter),
            fieldDelimiter(fieldDelimiter),
            skipLinesCount(skipLinesCount) {}

    CSVIterator<Args...> begin() const {
        CSVIterator<Args...> it(inputStream, lineDelimiter, valueDelimiter, fieldDelimiter);
        for(int i = 0; i < skipLinesCount; ++i){
            it++;
        }
        return it;
    }

    CSVIterator<Args...> end() const {
        return CSVIterator<Args...>(inputStream, true);
    }
};
