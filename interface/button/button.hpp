#ifndef BUTTON_HARD_HPP
#define BUTTON_HARD_HPP

#ifdef NEW_PAXO
    #define HOME_BUTTON 33
#endif
#ifdef OLD_PAXO
    #define HOME_BUTTON 32
#endif

#include "../../tasks/tasks.hpp"

class HomeButton
{
    public:
    void init()
    {
        #ifdef BUILD_PAXO
        pinMode(HOME_BUTTON, INPUT_PULLUP);
        #endif
        setInterval(new CallbackMethod<HomeButton>(this, &HomeButton::update), 10);
    }
    void update();
    void clear();
    bool pressed();

    uint state = 0;

    private:
    unsigned int button_chrono = 0;

    public:
    // standby mod

    uint64_t timer = 0;
    uint32_t timer_delay = 30000; // milliseconds

    void resetStandbyMod() { timer = millis(); }
    bool needStandbyMod() 
    {
        #ifndef BUILD_PAXO
            return false;
        #endif
        return timer_delay + timer < millis(); 
    }
};

HomeButton home_button;

#endif
