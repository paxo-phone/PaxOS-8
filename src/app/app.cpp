#ifndef APP_CPP
#define APP_CPP

#include "app.hpp"
#include "CAppsManager.hpp"

#include "phone/phone.hpp"
#include "message/message.hpp"
#include "calcul/calcul.hpp"
#include "hour/hour.hpp"
#include "map/map.hpp"
#include "2048/2048.hpp"
#include "snake/snake.hpp"
#include "browser/browser.hpp"
#include "minecraft/minecraft.hpp"

void initializeApplications()
{
    // Applications Natives
    CAppsManager::registerNativeApplication<Phone>();
    CAppsManager::registerNativeApplication<Message>();
    CAppsManager::registerNativeApplication<Contact>();
    CAppsManager::registerNativeApplication<Calcul>();
    CAppsManager::registerNativeApplication<Hour>();
    CAppsManager::registerNativeApplication<Map>();
    // CAppsManager::registerNativeApplication<Game_2048>();
    // CAppsManager::registerNativeApplication<Snake>();
    // CAppsManager::registerNativeApplication<Browser>();
    // CAppsManager::registerNativeApplication<Minecraft>();

    // Applications Lua
    const vector<string> luaAppNames = storage::listdir("apps/lua", true);
    for (const string& luaAppName : luaAppNames)
        CAppsManager::registerLuaApplication(luaAppName);
}

#endif
