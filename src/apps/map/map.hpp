#ifndef MAP_HPP
#define MAP_HPP

#include "../../drivers/drivers.hpp"
#include "../../modules/modules.hpp"

class Map : public CppAppContainer
{
    public:
    void main()
    {
        Window win("map");

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
