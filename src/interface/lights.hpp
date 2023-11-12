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
    void analog(int state);

    void ON();
    void OFF();
    void init();
};

extern ScreenLight screen_light;

#endif
