#ifndef APPS_HPP
#define APPS_HPP

#include "../widgets/gui.hpp"
#include "../interface/memory.hpp"

class App
{
    public:
    virtual void main() = 0;
};

struct AppData
{
    App* app = nullptr;
    std::string name;

    AppData(App* app, std::string name) : app(app), name(name) {}
};

#include "phone/phone.hpp"
#include "message/message.hpp"
#include "contact/contact.hpp"
#include "calcul/calcul.hpp"
#include "hour/hour.hpp"
#include "map/map.hpp"
#include "2048/2048.hpp"
#include "snake/snake.hpp"

AppData apps[] = {
    AppData (new Phone, "phone"),
    AppData (new Message, "message"),
    AppData (new Contact, "contact"),
    AppData (new Calcul, "calcul"),
    AppData (new Hour, "hour"),
    AppData (new Map, "map"),
    AppData (new Game_2048, "2048"),
    AppData (new Snake, "snake"),
};

#include "launcher.hpp"

#endif