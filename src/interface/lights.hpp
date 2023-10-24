#ifndef LIGHT_HPP
#define LIGHT_HPP

#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000

#define SCREEN_LED 25

class ScreenLight
{
    private:
    int STATE_LIGHT = 0;
    
    public:
    void analog(int state)
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

    void ON() { analog(255); }
    void OFF() { analog(-1); }
    void init()
    {
        #ifdef BUILD_PAXO
            ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
            ledcAttachPin(SCREEN_LED, PWM1_Ch);
        #endif
    }
};

ScreenLight screen_light;

#endif