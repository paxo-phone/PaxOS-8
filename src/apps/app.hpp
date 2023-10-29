#ifndef APPS_HPP
#define APPS_HPP

#include "../drivers/drivers.hpp"
#include "../modules/modules.hpp"

class App
{
    public:
    static std::vector<App*> appList;
    uint id;
    std::string name;
    std::string icon;

    virtual void run() = 0;
};

std::vector<App*> App::appList;

class CppAppContainer
{
    public:
    virtual void main() = 0;
};

template<class APP_T>

class CppApp : public App
{
    public:
    CppApp(std::string name)
    {
        this->name = name;
        icon = "apps/" + name + "/logo.png";
    }
    void run()
    {
        CppAppContainer* app = new APP_T;
        app->main();
        delete app;
    }
};

class LuaApp : public App
{
    public:
    LuaApp(std::string name)
    {
        conf = name+"/conf.txt";
        this->name = name;
        icon = "apps/lua/" + name + "/logo.png";
    }

    std::string conf;

    void run()
    {
        LuaInterpreter lua("apps/lua/"+name+"/");
        lua.loadScript("apps/lua/"+name+"/main.lua");
        if(lua.runApp() || lua.crashed)
        {
            Window win("error");
            Button* btn = new Button(0, 0, 290, 200, "Error during execution:\n"+lua.readError()+"\nExit");
            btn->setBackgroundColor(COLOR_ERROR);
            win.addChild(btn);

            while (!home_button.pressed() && btn->isTouched()==false)
            {
                win.updateAll();
            }
        }
    }
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
    //App::appList.push_back(new CppApp<Minecraft>("minecraft"));


    // lua
    vector<string> list = storage::listdir("apps/lua", true);
    for (auto app : list)
    {
        print(app);
        App::appList.push_back(new LuaApp(app));
    }
}

#include "launcher.hpp"

#endif
