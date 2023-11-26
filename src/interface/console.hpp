#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include "filestream.hpp"

class Console
{
    public:

    Console();
    ~Console();

    void log(char c);
    void log(const char* str);
    void log(const std::string& str);

    void error(char c);
    void error(const char* str);
    void error(const std::string& str);

    std::string input(const std::string& prompt);
    std::string input(void);
};

extern Console console;

#endif
