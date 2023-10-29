#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <stdlib.h>

#include "app.hpp"

void launcher()
{
    initializeApplications();

    Window win("launcher");
    
    win.setMarginX(0);
    win.setMarginY(CONTROL_BAR_SIZE);

    uint day_ = gsm.days;
    uint day = gsm.days;
    uint month = gsm.months;
    uint year = gsm.years;
    std::string dayName = daysOfWeek[(day+=month<3?year--:year-2,23*month/9+day+4+year/4-year/100+year/400)%7];
    std::string monthName = daysOfMonth[month==0?1:(month-1)];

    Label* label = new Label(AUTO, 31, AUTO, 55, dayName + "\n" + to_string(gsm.days) + " " + monthName);
    label->setHorizontalAlignment(CENTER_ALIGNMENT);
    label->fontHeight = 25;
    label->setColor(COLOR_GREY);
    label->setBackgroundColor(COLOR_LIGHT);
    win.addChild(label);

    std::vector<Gui*> appBoxs;

    for (int i = 0; i < App::appList.size(); i++)
    {
        Box* box = new Box(33 + (95 * (i%3)), 117 + (95 * int(i/3)), 63, 63);
        box->setBackgroundColor(COLOR_EXTRA_LIGHT);
        box->setRadius(15);

        Image* image = new Image(App::appList[i]->icon, AUTO, AUTO, AUTO, AUTO);
        image->load();
        box->addChild(image);

        win.addChild(box);

        appBoxs.push_back(image);
    }
    
    while(true)
    {
        win.updateAll();
        for (int i = 0; i < App::appList.size(); i++)
        {
            if(appBoxs[i]->isTouched())
            {
                App::appList[i]->run(); // launch application
                
                home_button.clear(); // clear the exit of the application
            }
        }

        if(day_ != gsm.days)
        {
            day_ = gsm.days;
            day = gsm.days;
            month = gsm.months;
            year = gsm.years;
            std::string dayName = daysOfWeek[(day+=month<3?year--:year-2,23*month/9+day+4+year/4-year/100+year/400)%7];
            std::string monthName = daysOfMonth[((month==0?1:(month-1)))%12];
            label->setText(dayName + "\n" + to_string(gsm.days) + " " + monthName);
        }

        if(home_button.pressed())
        {
            // standby mode
            screen_light.OFF();

            home_button.clear();

            #ifdef OLD_PAXO
                pinMode(22, OUTPUT); // 22 for new and 14 for old
                digitalWrite(22, 1);
            #endif

            #ifdef BUILD_PAXO
                setCpuFrequencyMhz(40);
                Serial.end();
                Serial.begin(115200);
                gsm.init();
            #endif

            while (!home_button.pressed())
            {
                eventHandler.update();
                home_button.resetStandbyMod();
                #ifdef BUILD_EMU
                    SDL_Delay(1);
                #endif
            }

            #ifdef BUILD_PAXO
                setCpuFrequencyMhz(240);
                Serial.end();
                Serial.begin(115200);
                gsm.init();
            #endif

            home_button.clear();

            screen_light.ON();
        }
        #ifdef BUILD_EMU
            SDL_Delay(20);
        #endif
    }
}

#endif
