#ifndef APPS_HPP
#define APPS_HPP

#include "../widgets/gui.hpp"
#include "../interface/filestream.hpp"
#include "../lua/lua.hpp"
#include "../interface/time.hpp"
#include "../interface/random.hpp"

#include "CApp.hpp"

class App
{
public:
    virtual void run() = 0;

    static std::vector<CApp*> appList;
    uint16_t id{};
    std::string name;
    std::string icon;
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
        lua.runApp();
    }
};

void initializeApplications();

#include "launcher.hpp"

#endif
