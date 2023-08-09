#ifndef HOUR_HPP
#define HOUR_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Hour : public App
{
    public:
    void main()
    {
        Window win("hour");

        while (!home_button.pressed())
        {
            win.updateAll();
            SDL_Delay(20);
        }
    }
};

#endif
