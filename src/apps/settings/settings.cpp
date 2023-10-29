//
//  settings.cpp
//
//  Created by Antoine Bollengier (github.com/b5i) on 10.08.2023.
//  Copyright © 2023 Antoine Bollengier. All rights reserved.
//  

#include "settings.hpp"

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
