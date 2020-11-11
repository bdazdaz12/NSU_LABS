#pragma once

#include <iostream>

enum class Trit : uint8_t{
    False = 0, //00
    Unknown = 1, //01
    True = 3 //11
};

struct tritSignificator { //почему-то по другому unordered map не работал
    uint8_t operator()(Trit trit) const {
        return static_cast<uint8_t>(trit);
    }
};
uint8_t tritSign(Trit trit);

Trit operator ! (Trit a);
Trit operator | (Trit a, Trit b);
void operator |= (Trit& a, Trit b);
Trit operator & (Trit a, Trit b);
void operator &= (Trit& a, Trit b);
std::ostream& operator << (std::ostream& outStream, Trit a);
