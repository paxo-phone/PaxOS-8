#ifndef C_LUA_APP_HPP
#define C_LUA_APP_HPP

#include <string>

#include "../lua/lua2.hpp"
#include "CApp.hpp"

class CLuaApp final : public CApp
{
    public:

        [[nodiscard]] const char* getAppName() const override
        {
            return m_luaAppName.c_str();
        }

        [[nodiscard]] std::string getAppBasePath() const override
        {
            std::string o = "";
            o += "apps/lua/";
            o += getAppName();
            o += "/";
            return o;
        }

        void initializeLuaApp(const std::string& luaAppName)
        {
            m_luaAppName = luaAppName;
        }

        void execute() override
        {
            LuaFile lua(getAppBasePath() + "/main.lua");  // test du nouveau lua
            lua.run();
        }

    private:
        std::string m_luaAppName;
};


#endif // C_LUA_APP_HPP
