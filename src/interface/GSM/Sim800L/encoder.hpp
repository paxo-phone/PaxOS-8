#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <string>
#include "../../shell.hpp"

class Encoder
{
    public:
    std::string convertEncodedCharToChar(int chr);
    int HexCharToDecimal(char chr);
    std::string HexStringToString(std::string str);
};

extern Encoder encoder;

#endif
