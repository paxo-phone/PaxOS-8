#ifndef WEBDRIVER_HPP
#define WEBDRIVER_HPP

#ifdef BUILD_PAXO

#include <HTTPClient.h>
#include <WiFi.h>

#endif

class Webdriver
{
    public:
    void connect()
    {
        #ifdef BUILD_PAXO
        WiFi.begin("Fairphone Gab", "bonjourbonjour");
        #endif
    }

    void disconnect()
    {
        #ifdef BUILD_PAXO
        WiFi.disconnect();
        #endif
    }

    void powerOff()
    {
        #ifdef BUILD_PAXO
        WiFi.mode(WIFI_OFF);
        #endif
    }

    void powerOn()
    {
        #ifdef BUILD_PAXO
        WiFi.mode(WIFI_STA);
        #endif
    }

    bool isConnected()
    {
        #ifdef BUILD_PAXO
        return WiFi.status() == WL_CONNECTED;
        #endif
        return true;
    }

    enum Mode
    {
        ALWAYS_DISABLED,
        AUTO_WIFI,
        ALWAYS_ENABLED
    };

    Mode mode = Mode::AUTO_WIFI;
};

#include "http/http.hpp"

#endif
