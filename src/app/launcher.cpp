#include "launcher.hpp"

#include "../interface/interface.hpp"

void launcher()
{
    initializeApplications();

    Window win("launcher");
    
    win.setMarginX(0);
    win.setMarginY(CONTROL_BAR_SIZE);

    uint16_t day_ = gsm.days;
    uint16_t day = gsm.days;
    uint16_t month = gsm.months;
    uint16_t year = gsm.years;
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
                
                // home_button::clear(); // clear the exit of the application
            }
        }

        if(day_ != gsm.days)
        {
            day_ = gsm.days;
            day = gsm.days;
            month = gsm.months;
            year = gsm.years;
            std::string dayName = daysOfWeek[(day+=month<3?year--:year-2,23*month/9+day+4+year/4-year/100+year/400)%7];
            std::string monthName = daysOfMonth[month==0?1:(month-1)];
            label->setText(dayName + "\n" + to_string(gsm.days) + " " + monthName);
        }

        if(home_button::isPressed())
        {
            // standby mode
            light::turnOff();

            // home_button::clear();

            // #ifdef OLD_PAXO
            //     pinMode(22, OUTPUT); // 22 for new and 14 for old
            //     digitalWrite(22, 1);
            // #endif

            #ifdef ESP32
                setCpuFrequencyMhz(40);
                Serial.end();
                Serial.begin(115200);
                gsm.init();
            #endif

            while (!home_button::isPressed())
            {
                eventHandler.update();
                // home_button::resetStandbyMod();
                #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                    SDL_Delay(1);
                #endif
            }

            #ifdef ESP32
                setCpuFrequencyMhz(240);
                Serial.end();
                Serial.begin(115200);
                gsm.init();
            #endif

            // home_button::clear();

            light::turnOn();
        }
        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            SDL_Delay(20);
        #endif
    }
}