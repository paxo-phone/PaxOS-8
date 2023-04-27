#ifndef BUTTON_HARD_HPP
#define BUTTON_HARD_HPP

#define HOME_BUTTON 33

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
};

HomeButton home_button;

#endif