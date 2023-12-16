//
// Created by Kevin on 16/12/2023.
//

#ifndef PAXOS_CLUAAPP_HPP
#define PAXOS_CLUAAPP_HPP

#include <string>

#include "../lua/lua.hpp"
#include "CApp.hpp"

class CLuaApp : public CApp
{
public:
    [[nodiscard]] const char* getAppName() const override
    {
        return m_luaAppName.c_str();
    }

    [[nodiscard]] std::string getAppBasePath() const override
    {
        std::stringstream ss;
        ss << "apps/lua/";
        ss << getAppName();
        ss << "/";
        return ss.str();
    }

    void initializeLuaApp(const std::string& luaAppName)
    {
        m_luaAppName = luaAppName;
    }

    void onApplicationStart() override
    {
        LuaInterpreter lua(getAppBasePath());
        lua.loadScript(getAppBasePath() + "/main.lua");
        lua.runApp();
    }

private:
    std::string m_luaAppName;
};


#endif //PAXOS_CLUAAPP_HPP
