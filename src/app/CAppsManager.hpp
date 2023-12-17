//
// Created by Kevin on 16/12/2023.
//

#ifndef PAXOS_CAPPSMANAGER_HPP
#define PAXOS_CAPPSMANAGER_HPP

#include <vector>
#include <iostream>
#include <memory>

#include "CApp.hpp"
#include "CLuaApp.hpp"

using LuaAppUniquePtr = std::unique_ptr<CLuaApp>;
using AppUniquePtr = std::unique_ptr<CApp>;
using AppVector = std::vector<AppUniquePtr>;

class CAppsManager
{
public:
    static CAppsManager& getInstance()
    {
        static CAppsManager sInstance;
        return sInstance;
    }
    static const AppVector& getApplications()
    {
        return getInstance().m_apps;
    }

    template <typename T>
    static void registerNativeApplication()
    {
        AppUniquePtr pNewRegisteredApp = std::make_unique<T>();
        std::cout << "Successfully registered '" << pNewRegisteredApp->getAppName() << "' application." << std::endl;

        getInstance().m_apps.push_back(std::move(pNewRegisteredApp));
    }

    static void registerLuaApplication(const std::string& luaAppName)
    {
        LuaAppUniquePtr pNewRegisteredLuaApp = std::make_unique<CLuaApp>();
        pNewRegisteredLuaApp->initializeLuaApp(luaAppName);

        std::cout << "Successfully registered '" << pNewRegisteredLuaApp->getAppName() << "' Lua application." << std::endl;

        getInstance().m_apps.push_back(std::move(pNewRegisteredLuaApp));
    }

private:
    AppVector m_apps;
};


#endif //PAXOS_CAPPSMANAGER_HPP
