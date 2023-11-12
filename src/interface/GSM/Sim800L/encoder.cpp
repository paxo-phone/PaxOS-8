//
//  encoder.cpp
//  PaxOS8
//
//  Created by Antoine Bollengier on 12.11.2023.
//

#include "encoder.hpp"

std::string Encoder::convertEncodedCharToChar(int chr)
{
    print(std::to_string(chr));
    switch (chr)
    {
        case 225: return "a";
        case 226: return "a";
        case 224: return "a";
        case 192: return "A";
        case 193: return "A";
        case 194: return "A";
        case 195: return "A";
        case 197: return "A";
        case 232: return "e";
        case 233: return "e";
        case 234: return "e";
        case 235: return "e";
        case 281: return "e";
        case 279: return "e";
        case 275: return "e";
        case 200: return "E";
        case 201: return "E";
        case 202: return "E";
        case 203: return "E";
        case 280: return "E";
        case 278: return "E";
        case 274: return "E";
        case 363: return "u";
        case 250: return "u";
        case 252: return "u";
        case 249: return "u";
        case 251: return "u";
        case 362: return "U";
        case 218: return "U";
        case 220: return "U";
        case 217: return "U";
        case 219: return "U";
        case 299: return "i";
        case 303: return "i";
        case 237: return "i";
        case 236: return "i";
        case 239: return "i";
        case 238: return "i";
        case 298: return "I";
        case 302: return "I";
        case 205: return "I";
        case 204: return "I";
        case 207: return "I";
        case 206: return "I";
        case 186: return "o";
        case 349: return "o";
        case 248: return "o";
        case 245: return "o";
        case 243: return "o";
        case 242: return "o";
        case 246: return "o";
        case 244: return "o";
        case 339: return "N";
        case 338: return "OE";
        case 348: return "O";
        case 216: return "O";
        case 213: return "O";
        case 211: return "O";
        case 210: return "O";
        case 214: return "O";
        case 231: return "c";
        case 263: return "c";
        case 269: return "c";
        case 199: return "C";
        case 262: return "C";
        case 268: return "C";
        case 341: return "n";
        case 241: return "n";
        case 209: return "N";
    }
    std::string out = "";
    out+=char(chr);
    return out;
}

int Encoder::HexCharToDecimal(char chr)
{
    int out = 0;
    switch (chr)
    {
        case '0':
            out=0;
        break;
        case '1':
            out=1;
        break;
        case '2':
            out=2;
        break;
        case '3':
            out=3;
        break;
        case '4':
            out=4;
        break;
        case '5':
            out=5;
        break;
        case '6':
            out=6;
        break;
        case '7':
            out=7;
        break;
        case '8':
            out=8;
        break;
        case '9':
            out=9;
        break;
        case 'A':
            out=10;
        break;
        case 'B':
            out=11;
        break;
        case 'C':
            out=12;
        break;
        case 'D':
            out=13;
        break;
        case 'E':
            out=14;
        break;
        case 'F':
            out=15;
        break;
        default:
            out=char('0');
        break;
    }
    return char(out);
}

std::string Encoder::HexStringToString(std::string str)
{
    std::string result = "";
    print("analyze...");

    print(str);

    for(int i=0; i<str.length(); i++)
    {
        result+=convertEncodedCharToChar(str[i]);
    }

    print(result);

    str=result;
    result="";

    if(str.find("00")==-1)
        return str;

    result = "";

    for (int i = 0; i + 3 < str.length(); i+=4)
    {
        print(str.substr(i, 3));
        if(str.substr(i, 3) == "D83") // emoj detected for the next 8 characters
        {
            result+=str.substr(i, 8);
            i+=4;
        }else
        {
            int chr = int((HexCharToDecimal(str[i])*16*16*16)+(HexCharToDecimal(str[i+1])*16*16)+(HexCharToDecimal(str[i+2])*16)+HexCharToDecimal(str[i+3]));
            print("char " + std::to_string(i/4) + " : " + char(str[i]) + char(str[i+1])+char(str[i+2])+char(str[i+3]) + " : " + char(HexCharToDecimal(str[i])) + "," + char(HexCharToDecimal(str[i+1])) + "," + char(HexCharToDecimal(str[i+2])) + "," + char(HexCharToDecimal(str[i+3])) + " : " + char(chr));
            if(chr>31&&chr<127) // valid ascii character
            {
                result+=char(chr);
            }
            else
            {
                result+=convertEncodedCharToChar(chr);    // invalid ascii character -> decode
            }
        }
    }
    return result;
}

Encoder encoder;
