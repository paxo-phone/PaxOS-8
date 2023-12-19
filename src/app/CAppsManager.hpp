#ifndef C_APPS_MANAGER_HPP
#define C_APPS_MANAGER_HPP

#include <vector>
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
            getInstance().m_apps.push_back(std::move(pNewRegisteredApp));
        }

        static void registerLuaApplication(const std::string& luaAppName)
        {
            auto pNewRegisteredLuaApp = std::make_unique<CLuaApp>();
            pNewRegisteredLuaApp->initializeLuaApp(luaAppName);
            getInstance().m_apps.push_back(std::move(pNewRegisteredLuaApp));
        }

    private:
        AppVector m_apps;
};


#endif // C_APPS_MANAGER_HPP
