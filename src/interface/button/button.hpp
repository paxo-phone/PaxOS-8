#ifndef BUTTON_HARD_HPP
#define BUTTON_HARD_HPP

#ifdef NEW_PAXO
    #define HOME_BUTTON 33
#endif
#ifdef OLD_PAXO
    #define HOME_BUTTON 32
#endif

#include "../interface.hpp"

class HomeButton
{
    public:
    void init();
    void update();
    void clear();
    bool pressed();

    uint16_t state = 0;

    private:
    unsigned int button_chrono = 0;

    public:
    // standby mod

    uint64_t timer = 0;
    uint32_t timer_delay = 30000; // milliseconds

    void resetStandbyMod();
    bool needStandbyMod();
};

HomeButton home_button;

#endif
