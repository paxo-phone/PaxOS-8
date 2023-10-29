#ifndef HOUR_HPP
#define HOUR_HPP

#include "../../drivers/drivers.hpp"
#include "../../modules/modules.hpp"

class Hour : public CppAppContainer
{
    public:
    void main()
    {
        Window win("hour");

        while (!home_button.pressed())
        {
            win.updateAll();
            #ifdef BUILD_EMU
                SDL_Delay(20);
            #endif
        }
    }
};

#endif
