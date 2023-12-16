#ifndef APP_CPP
#define APP_CPP

#include "app.hpp"

#include "phone/phone.hpp"
#include "message/message.hpp"

std::vector<CApp*> App::appList;

void initializeApplications()
{
    // cpp
    App::appList.push_back(new Phone());
    App::appList.push_back(new Message());
    //App::appList.push_back(new CppApp<Contact>("contact"));
    //App::appList.push_back(new CppApp<Calcul>("calcul"));
    //App::appList.push_back(new CppApp<Hour>("hour"));
    //App::appList.push_back(new CppApp<Map>("map"));
    //App::appList.push_back(new CppApp<Game_2048>("2048"));
    //App::appList.push_back(new CppApp<Snake>("snake"));
    //App::appList.push_back(new CppApp<Browser>("browser"));
    //App::appList.push_back(new CppApp<Minecraft>("minecraft"));

    // lua


    //const vector<string>& list = storage::listdir("apps/lua", true);
    //for (const string& app : list)
    //    App::appList.push_back(new LuaApp(app));
}

#endif
