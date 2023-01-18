#include "memory.hpp"

using namespace std; 

LFile::LFile(std::string filename)
{
    this->filename = filename;
}

#ifdef BUILD_EMU

#include <fstream>

bool LFile::open(MemoryOpenMode mode)
{
    /*file = new ofstream(filename.c_str());
    if(file)
        return true;*/
    return false;
}

string LFile::read()
{
    ifstream f(filename.c_str());
    if(f)
    {
        std::string ligne { "" };
        std::string text { "" };

        while (std::getline(f, ligne))
        {
            text=text+ligne;
        }

        return text;
    }
    else
    {
        return "error";
    }
}

void LFile::write(char chr)
{
    ofstream f(filename.c_str(), std::ios::app);
    f << chr;
    f.clear();
}

#endif