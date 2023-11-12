#ifndef APPS_HPP
#define APPS_HPP

#include "../widgets/gui.hpp"
#include "../interface/filestream.hpp"
#include "../lua/lua.hpp"

class App
{
    public:
    static std::vector<App*> appList;
    uint16_t id;
    std::string name;
    std::string icon;

    virtual void run() = 0;
};


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
        lua.runApp();
    }
};

void initializeApplications();

#include "launcher.hpp"

#endif
