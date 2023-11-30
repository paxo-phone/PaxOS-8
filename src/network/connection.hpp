#ifndef CONNECTION_HPP
#define CONNECTION_HPP

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
#include <stdint.h>
#include <string.h>

namespace network
{
    void connect(const std::string& ssid);
    void connect(const std::string& ssid, const std::string& passwd); // pas de sécurité pour le moment
    
    void disconnect(void);

    void turnOFF(void);
    void turnON(void);
    
    bool isConnected(void);

    // getIP(...)
    // ...
}

#endif /* CONNECTION_HPP */
