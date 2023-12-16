#ifndef MAP_HPP
#define MAP_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/filestream.hpp"
#include "../app.hpp"

class Map : public CApp
{

public:
    [[nodiscard]] const char* getAppName() const override { return "Phone"; }
    void main() override
    {
        Window win("map");

        while (!home_button::isPressed())
        {
            win.updateAll();
            #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                SDL_Delay(20);
            #endif
        }
    }
};

#endif
