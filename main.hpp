//#define BUILD_PAXO
#define BUILD_EMU 1

#include <stdlib.h>
#include <iostream>

using namespace std;

#include "interface/interface.hpp"
#include "widgets/gauss.hpp"

#ifdef BUILD_EMU

#include "interface/LovyanGFX/LGFX_SDL.cpp"

#endif

Box* box = nullptr;
bool state = false;

void setup()
{
    Gauss::initScreen();

    box = new Box(10, 10, 100, 100);
    box->setBackgroundColor(COLOR_ERROR);

    box->drawAll();
}

void loop()
{
    box->updateAll();
    if(box->isTouched())
    {
        cout << touch.getX() << " " << touch.getY() << endl;
        if(state)
            box->setBackgroundColor(COLOR_GREY);
        else
            box->setBackgroundColor(COLOR_BLUE);
        state=!state;
        box->drawAll();
    }
}