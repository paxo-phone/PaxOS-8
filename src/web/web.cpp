#include "web.hpp"

void Webdriver::connect()
{
    #ifdef ESP32
    WiFi.begin("Fairphone Gab", "bonjourbonjour");
    #endif
}

void Webdriver::disconnect()
{
    #ifdef ESP32
    WiFi.disconnect();
    #endif
}

void Webdriver::powerOff()
{
    #ifdef ESP32
    WiFi.mode(WIFI_OFF);
    #endif
}

void Webdriver::powerOn()
{
    #ifdef ESP32
    WiFi.mode(WIFI_STA);
    #endif
}

bool Webdriver::isConnected()
{
    #ifdef ESP32
    return WiFi.status() == WL_CONNECTED;
    #endif
    return true;
}

Webdriver webdriver;