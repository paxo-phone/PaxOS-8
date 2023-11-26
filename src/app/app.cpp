#ifndef APP_CPP
#define APP_CPP

#include "app.hpp"

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
#include "browser/browser.hpp"

std::vector<App*> App::appList;

void initializeApplications()
{
    // cpp
    App::appList.push_back(new CppApp<Phone>("phone"));
    App::appList.push_back(new CppApp<Message>("message"));
    App::appList.push_back(new CppApp<Contact>("contact"));
    App::appList.push_back(new CppApp<Calcul>("calcul"));
    App::appList.push_back(new CppApp<Hour>("hour"));
    App::appList.push_back(new CppApp<Map>("map"));
    App::appList.push_back(new CppApp<Game_2048>("2048"));
    App::appList.push_back(new CppApp<Snake>("snake"));
    App::appList.push_back(new CppApp<Browser>("browser"));
    //App::appList.push_back(new CppApp<Minecraft>("minecraft"));

    // lua
    vector<string> list = storage::listdir("apps/lua", true);
    for (string app : list)
        App::appList.push_back(new LuaApp(app));
}

#endif
