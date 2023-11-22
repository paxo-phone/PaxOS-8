#ifndef WEBDRIVER_HPP
#define WEBDRIVER_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#ifdef ESP32

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
