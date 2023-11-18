#include <stdint.h>

#include "lights.hpp"

namespace light
{
    #define LED_PIN 25

    #define PWM1_CH    0
    #define PWM1_RES   8
    #define PWM1_FREQ  1000

    #define ANALOG_STATE_MAX 255
    #define ANALOG_STATE_MIN -1

    void init(void)
    {
        #ifdef BUILD_PAXO
            ledcSetup(PWM1_CH, PWM1_FREQ, PWM1_RES);
            ledcAttachPin(LED_PIN, PWM1_CH);
        #endif
    }

    void setAnalogState(int16_t targetState)
    {
        #ifdef BUILD_PAXO
        
        static int16_t currentState = 0;

        if (currentState > targetState)
        {
            while (currentState > targetState)
            {
                ledcWrite(PWM1_CH, currentState--);
                delay(1);
            }
        }
        else
        {
            while (currentState < targetState)
            {
                ledcWrite(PWM1_CH, currentState++);
                delay(1);
            }
        }

        #endif /* BUILD_PAXO */
    }

    void turnOn(void) 
    { 
        setAnalogState(ANALOG_STATE_MAX); 
    }

    void turnOff(void) 
    { 
        setAnalogState(ANALOG_STATE_MIN); 
    }

    void setState(bool state)
    {
        state == true ? turnOn() : turnOff();
    }
}