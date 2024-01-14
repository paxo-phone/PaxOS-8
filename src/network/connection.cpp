#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#ifdef ESP32
    #include <HTTPClient.h>
    #include <WiFi.h>
#endif

#include <string>

#include "connection.hpp"

namespace network
{
    // enum Mode
    // {
    //     ALWAYS_DISABLED,
    //     AUTO_WIFI,
    //     ALWAYS_ENABLED
    // };

    // Mode mode = Mode::AUTO_WIFI;

    void connect(const std::string& ssid)
    {
        #ifdef ESP32
            WiFi.begin(ssid.c_str());
        #endif
    }

    void connect(const std::string& ssid, const std::string& passwd)
    {
        #ifdef ESP32
            WiFi.begin(ssid.c_str(), password.c_str());
        #endif
    }

    void disconnect(void)
    {
        #ifdef ESP32
            WiFi.disconnect();
        #endif
    }

    void turnOFF(void)
    {
        #ifdef ESP32
            WiFi.mode(WIFI_OFF);
        #endif
    }

    void turnON(void)
    {
        #ifdef ESP32
            WiFi.mode(WIFI_STA);
        #endif
    }

    bool isConnected(void)
    {
        #ifdef ESP32
            return WiFi.status() == WL_CONNECTED;
        #endif

        return true;
    }


}