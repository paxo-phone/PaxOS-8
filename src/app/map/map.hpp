#ifndef MAP_HPP
#define MAP_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/filestream.hpp"
#include "../app.hpp"


class Map : public CppAppContainer
{
    public:
    void main()
    {
        Window win("map");

        while (!home_button.pressed())
        {
            win.updateAll();
            #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                SDL_Delay(20);
            #endif
        }
    }
};

#endif
