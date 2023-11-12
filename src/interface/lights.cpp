//
//  lights.cpp
//  PaxOS8
//
//  Created by Antoine Bollengier on 12.11.2023.
//

#include "interface.hpp"

void ScreenLight::analog(int state)
{
    if (state < STATE_LIGHT)
    {
        while (STATE_LIGHT > state)
        {
            #ifdef BUILD_PAXO
                ledcWrite(PWM1_Ch, STATE_LIGHT--);
                delay(1);
            #else
                STATE_LIGHT--;
            #endif
        }
        return;
    }
    if (state > STATE_LIGHT)
    {
        while (STATE_LIGHT < state)
        {
            #ifdef BUILD_PAXO
                ledcWrite(PWM1_Ch, STATE_LIGHT++);
                delay(1);
            #else
                STATE_LIGHT++;
            #endif
        }
        return;
    }
}

void ScreenLight::ON() { analog(255); }
void ScreenLight::OFF() { analog(-1); }
void ScreenLight::init()
{
    #ifdef BUILD_PAXO
        ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
        ledcAttachPin(SCREEN_LED, PWM1_Ch);
    #endif
}

ScreenLight screen_light;
