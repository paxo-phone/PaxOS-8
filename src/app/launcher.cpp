#include "launcher.hpp"

#include "CAppsManager.hpp"
#include "ApplicationsRegistery.hpp"

void launcher()
{
    // Ajouter les applications au registre
    registerApplications();

    // Récupérer les applications ajoutées
    const auto& registeredApplications = CAppsManager::getApplications();

    Window win("launcher");
    win.enableToolbar();

    uint16_t day_ = gsm.days;
    uint16_t day = gsm.days;
    uint16_t month = gsm.months;
    uint16_t year = gsm.years;
    std::string dayName = daysOfWeek[(day+=month<3?year--:year-2,23*month/9+day+4+year/4-year/100+year/400)%7];
    std::string monthName = daysOfMonth[month==0?1:(month-1)];

    Label* label = new Label(AUTO, 31, AUTO, AUTO, dayName + "\n" + to_string(gsm.days) + " " + monthName);
    label->setHorizontalAlignment(CENTER_ALIGNMENT);
    label->setFontSize(25);
    label->setFontName("sans");
    label->setColor(COLOR_BLACK);
    label->setBackgroundColor(COLOR_LIGHT);
    win.addChild(label);

    std::vector<Gui*> appBoxs;

    for (int i = 0; i < registeredApplications.size(); i++)
    {
        std::cout << registeredApplications[i]->getAppIconPath() << std::endl;
        Box* box = new Box(33 + (95 * (i%3)), 117 + (95 * int(i/3)), 63, 63);
        box->setBackgroundColor(COLOR_EXTRA_LIGHT);
        box->setRadius(15);

        Image* image = new Image(registeredApplications[i]->getAppIconPath(), AUTO, AUTO, AUTO, AUTO);
        image->load();
        box->addChild(image);

        win.addChild(box);

        appBoxs.push_back(image);
    }

    win.addChild(label);

    while(true)
    {
        win.updateAll();
        for (int i = 0; i < registeredApplications.size(); i++)
        {
            if(appBoxs[i]->isTouched())
            {
                registeredApplications[i]->execute(); // launch application
            }
        }

        if(day_ != gsm.days)
        {
            std::string monthName = daysOfMonth[gsm.months==0?0:(gsm.months-1)];
            Date d = {gsm.days, gsm.months, gsm.years};
            label->setText(std::string(daysOfWeek[myWhatDay(d)]) + "\n" + to_string(gsm.days) + " " + monthName);
            day_ = gsm.days;
        }

        if(home_button::isPressed())
        {

            light::turnOff();
            standbymode::enable();

            while (!home_button::isPressed())
            {
                eventHandler.update();

                #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                    SDL_Delay(1);
                #endif
            }

            standbymode::disable();
            light::turnOn();

        }
        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            SDL_Delay(20);
        #endif
    }
}