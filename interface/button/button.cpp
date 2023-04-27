#ifndef BUTTON_HARD_CPP
#define BUTTON_HARD_CPP

#include "button.hpp"

void HomeButton::update()
{
    #ifdef BUILD_PAXO
    bool input = !digitalRead(HOME_BUTTON);
    #endif
    #ifdef BUILD_EMU
    bool input = false;
    #endif

    print(to_string(input));

    if (!input && state == 1)
    {
        state = 2;
        return;
    }

    if (input && state == 0)
    {
        state = 1;
        return;
    }

    if (input && state == 1);
    {
        // do nothing
    }
}

bool HomeButton::pressed()
{
    return state == 2;
}

void HomeButton::clear()
{
    state = 0;
}

#endif