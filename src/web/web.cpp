#include "web.hpp"

void Webdriver::connect()
{
    #ifdef BUILD_PAXO
    WiFi.begin("Fairphone Gab", "bonjourbonjour");
    #endif
}

void Webdriver::disconnect()
{
    #ifdef BUILD_PAXO
    WiFi.disconnect();
    #endif
}

void Webdriver::powerOff()
{
    #ifdef BUILD_PAXO
    WiFi.mode(WIFI_OFF);
    #endif
}

void Webdriver::powerOn()
{
    #ifdef BUILD_PAXO
    WiFi.mode(WIFI_STA);
    #endif
}

bool Webdriver::isConnected()
{
    #ifdef BUILD_PAXO
    return WiFi.status() == WL_CONNECTED;
    #endif
    return true;
}

Webdriver webdriver;