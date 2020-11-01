#pragma once

#include <iostream>

enum class Trit : uint8_t{
    False = 0, //00
    Unknown = 1, //01
    True = 3 //11
};

Trit operator ! (Trit a);
Trit operator | (Trit a, Trit b);
Trit operator & (Trit a, Trit b);
std::ostream& operator << (std::ostream& outStream, Trit a);
