#ifndef HOUR_HPP
#define HOUR_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/filestream.hpp"
#include "../app.hpp"


class Hour : public CppAppContainer
{
    public:
    void main()
    {
        Window win("hour");

        Label2* label = new Label2(0, 0, 200, 200, "// facteur entre le buffer de rendu et les mesures réelles sur l'écran");
        label->setBorderSize(5);
        label->setRadius(20);
        label->setFontSize(15);
        label->setItalic(true);
        label->setBold(true);
        label->setHorizontalAlignment(RIGHT_ALIGNMENT);
        label->enabledBackground = true;
        win.addChild(label);

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
