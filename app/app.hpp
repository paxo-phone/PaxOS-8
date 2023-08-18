#ifndef APPS_HPP
#define APPS_HPP

#include "../widgets/gui.hpp"
#include "../interface/memory.hpp"

class App
{
    public:
    virtual void main() = 0;
    static std::shared_ptr<App> createInstance();
};

struct AppData
{
    std::shared_ptr<App> (*createInstance)();
    std::string name;

    AppData(std::shared_ptr<App> (*createInstance)(), std::string name) {
        this->createInstance = createInstance;
        this->name = name;
    }
//    App* app = nullptr;
//    std::string name;
//
//    AppData(App* app, std::string name) : app(app), name(name) {}
};

#include "phone/phone.hpp"
#include "message/message.hpp"
#include "contact/contact.hpp"
#include "calcul/calcul.hpp"
#include "hour/hour.hpp"
#include "map/map.hpp"
#include "minecraft/minecraft.hpp"
#include "2048/2048.hpp"
#include "settings/settings.cpp"
#include "snake/snake.hpp"

AppData apps[] = {
    AppData (Phone::createInstance, "phone"),
    AppData (Message::createInstance, "message"),
    AppData (Contact::createInstance, "contact"),
    AppData (Calcul::createInstance, "calcul"),
    AppData (Hour::createInstance, "hour"),
    AppData (Map::createInstance, "map"),
    AppData (Game_2048::createInstance, "2048"),
    AppData (Snake::createInstance, "snake"),
    AppData (Minecraft::createInstance, "settings")
};

//AppData apps[] = {
//    AppData (new Phone, "phone"),
//    AppData (new Message, "message"),
//    AppData (new Contact, "contact"),
//    AppData (new Calcul, "calcul"),
//    AppData (new Hour, "hour"),
//    AppData (new Map, "map"),
//    AppData (new Game_2048, "2048"),
//    AppData (new Snake, "snake"),
//    AppData (new Minecraft, "settings")
//};

#include "launcher.hpp"

#endif
