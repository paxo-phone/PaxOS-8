#ifndef MAP_HPP
#define MAP_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Map : public App
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
