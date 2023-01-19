//#define BUILD_PAXO
#define BUILD_EMU 1

#include <stdlib.h>
#include <iostream>

using namespace std;

#include "interface/interface.hpp"
#include "widgets/gauss.hpp"

void setup()
{
}

void loop()
{
}

#ifdef BUILD_EMU

int main(int argc, char* argv[])
{
    setup();

    while(true)
        loop();
}

#endif
