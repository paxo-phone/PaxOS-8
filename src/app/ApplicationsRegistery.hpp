#ifndef APPLICATIONS_REGISTERY_HPP
#define APPLICATIONS_REGISTERY_HPP

#include "phone/phone.hpp"
#include "message/message.hpp"
#include "calcul/calcul.hpp"
#include "hour/hour.hpp"
#include "map/map.hpp"
#include "2048/2048.hpp"
#include "snake/snake.hpp"
#include "browser/browser.hpp"

#include "CAppsManager.hpp"

void registerApplications(void)
{
    // Natives

    CAppsManager::registerNativeApplication<Phone>();
    CAppsManager::registerNativeApplication<Message>();
    CAppsManager::registerNativeApplication<Contact>();
    CAppsManager::registerNativeApplication<Calcul>();
    CAppsManager::registerNativeApplication<Hour>();
    CAppsManager::registerNativeApplication<Map>();
    CAppsManager::registerNativeApplication<Game_2048>();
    CAppsManager::registerNativeApplication<Snake>();
    CAppsManager::registerNativeApplication<Browser>();

    // Externes
    const std::vector<std::string> luaAppNames = storage::listdir("apps/lua", true);
    for (const std::string& luaAppName : luaAppNames)
        CAppsManager::registerLuaApplication(luaAppName);
}

#endif /* APPLICATIONS_REGISTERY_HPP */