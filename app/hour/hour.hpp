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
            #ifdef BUILD_EMU
                SDL_Delay(20);
            #endif
        }
    }
    
    static std::shared_ptr<App> createInstance() {
        return std::make_shared<Hour>();
    }
};

#endif
