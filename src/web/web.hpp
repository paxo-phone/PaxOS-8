#ifndef WEBDRIVER_HPP
#define WEBDRIVER_HPP

#include "../includes.h"

#ifdef BUILD_PAXO

#include <HTTPClient.h>
#include <WiFi.h>

#endif

class Webdriver
{
    public:
    void connect();
    void disconnect();
    void powerOff();
    void powerOn();
    bool isConnected();

    enum Mode
    {
        ALWAYS_DISABLED,
        AUTO_WIFI,
        ALWAYS_ENABLED
    };

    Mode mode = Mode::AUTO_WIFI;
};

extern Webdriver webdriver;

#include "http/http.hpp"

#endif
