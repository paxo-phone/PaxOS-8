#ifndef BUTTON_HARD_CPP
#define BUTTON_HARD_CPP

#include "button.hpp"

#include "../../tasks/tasks.hpp"
#include "../../widgets/gui.hpp"
#include "../interface.hpp"

void HomeButton::init()
{
    #ifdef BUILD_PAXO
    pinMode(HOME_BUTTON, INPUT_PULLUP);
    #endif
    setInterval(new CallbackMethod<HomeButton>(this, &HomeButton::update), 10);
}

void HomeButton::update()
{
    #ifdef BUILD_PAXO
    bool input = !digitalRead(HOME_BUTTON);
    #else
    bool input = false;
    #endif

    if ((!input && state == 1) || needStandbyMod())
    {
        state = 2;
        resetStandbyMod();
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

void HomeButton::resetStandbyMod() 
{
    timer = millis();
}

bool HomeButton::needStandbyMod() 
{
    #ifndef BUILD_PAXO
        return false;
    #endif
    return timer_delay + timer < millis(); 
}

#endif
