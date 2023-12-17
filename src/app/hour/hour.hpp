#ifndef HOUR_HPP
#define HOUR_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/filestream.hpp"

#include "../CApp.hpp"

class Hour : public CApp
{

public:
    [[nodiscard]] const char* getAppName() const override { return "Hour"; }
    void main() override
    {
        Window win("hour");

        Label* label = new Label(0, 0, 200, 200, "// facteur entre le buffer de rendu et les mesures réelles sur l'écran");
        label->setBorderSize(5);
        label->setRadius(20);
        label->setFontSize(15);
        label->setVerticalAlignment(DOWN_ALIGNMENT);
        label->setItalic(true);
        label->setBold(true);
        label->setHorizontalAlignment(RIGHT_ALIGNMENT);
        label->enabledBackground = true;
        win.addChild(label);

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
