#include "settings.hpp"

#include "../CApp.hpp"

class Settings : public CApp
{

public:
    [[nodiscard]] const char* getAppName() const override { return "Settings"; }
    void main() override
    {
        Window win("settings");
        
        while (!home_button::isPressed())
        {
            win.updateAll();
            #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                SDL_Delay(20);
            #endif
        }
    }
};
