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
            #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                SDL_Delay(20);
            #endif
        }
    }
};
