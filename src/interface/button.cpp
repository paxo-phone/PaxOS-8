#ifndef BUTTON_HARD_CPP
#define BUTTON_HARD_CPP

#include "button.hpp"

#include "../../tasks/tasks.hpp"
#include "../../widgets/gui.hpp"
#include "../interface.hpp"

void HomeButton::init()
{
    #ifdef ESP32
    pinMode(HOME_BUTTON_PIN, INPUT_PULLUP);
    #endif
    setInterval(new CallbackMethod<HomeButton>(this, &HomeButton::update), 10);
}

void HomeButton::update()
{
    #ifdef ESP32
    bool input = !digitalRead(HOME_BUTTON_PIN);
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
    #ifndef ESP32
        return false;
    #endif
    return timer_delay + timer < millis(); 
}

HomeButton HOME_BUTTON_PIN;
#endif
