#include "settings.hpp"
#include "../app.hpp"

class Settings : public CppAppContainer
{
    public:
    void main()
    {
        Window win("settings");
        
        while (!home_button.pressed())
        {
            win.updateAll();
            #ifdef BUILD_EMU
                SDL_Delay(20);
            #endif
        }
    }
};
