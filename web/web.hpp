#ifndef WEBDRIVER_HPP
#define WEBDRIVER_HPP

#include <HTTPClient.h>
#include <WiFi.h>

class Webdriver
{
    public:
    void connect()
    {
        WiFi.begin("Fairphone Gab", "bonjourbonjour");
    }

    void disconnect()
    {
        WiFi.disconnect();
    }

    void powerOff()
    {
        WiFi.mode(WIFI_OFF);
    }

    void powerOn()
    {
        WiFi.mode(WIFI_STA);
    }

    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    requests

    enum Mode
    {
        ALWAYS_DISABLED,
        AUTO,
        ALWAYS_ENABLED
    };

    Mode mode = AUTO;
};

Webdriver webdriver;

#include "http/http.hpp"

#endif