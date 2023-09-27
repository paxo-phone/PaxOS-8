#ifndef BROWSER_HPP
#define BROWSER_HPP

#include "../../widgets/gui/iframe/iframe.hpp"
#include "../../interface/memory.hpp"

class Browser : public CppAppContainer
{
    public:
    void main()
    {
        Window win("browser");
        win.setMarginX(0);

        std::string html = "<!DOCTYPE html>"
                           "<html>"
                           "    <head>"
                           "        <title>Page title</title>"
                           "    </head>"
                           "    <body>"
                           "        <h1>My title</h1>"
                           "        <p>Lorem ipsum</p>"
                           "    </body>"
                           "</html>";

        storage::LFile file("apps/browser/data.html", storage::OPEN_MODE::READ);
        html = file.read();
        file.close();

        Iframe* iframe = new Iframe(html, 0, 0, 310, 480-CONTROL_BAR_SIZE-win.getMarginY());
        win.addChild(iframe);


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
