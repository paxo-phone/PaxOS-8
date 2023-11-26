#include <iostream>
#include <string>
#include <cstring>

#include "console.hpp"

#include "interface.hpp"
#include "GSM/GSM.hpp"

#define SERIAL_DATA_RATE 115200

Console::Console()
{
    #ifdef ESP32
        Serial.begin(SERIAL_DATA_RATE);
    #endif 
}

Console::~Console()
{}

void Console::log(char c)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        std::cout << c << std::endl;
    #endif

    #ifdef ESP32
        Serial.print(c);
        Serial.println();
    #endif
}

void Console::log(const char* str)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        std::cout << str << std::endl;
    #endif

    #ifdef ESP32
        for (int i = 0; i < strlen(str); i++)
            Serial.print(str[i]);

        Serial.println();
    #endif
}

void Console::log(const std::string& str)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        std::cout << str << std::endl;
    #endif

    #ifdef ESP32
        for (int i = 0; i < str.size(); i++)
            Serial.print(str[i]);

        Serial.println();
    #endif
}

void Console::error(char c)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        std::cerr << c << std::endl;
    #endif

    #ifdef ESP32
        Serial.print(c);
        Serial.println();
    #endif
}

void Console::error(const char* str)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        std::cerr << str << std::endl;
    #endif

    #ifdef ESP32
        for (int i = 0; i < strlen(str); i++)
            Serial.print(str[i]);
        Serial.println();
    #endif
}

void Console::error(const std::string& str)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        std::cerr << str << std::endl;
    #endif

    #ifdef ESP32
        for (int i = 0; i < str.size(); i++)
            Serial.print(str[i]);
        Serial.println();
    #endif
}

std::string Console::input(const std::string& prompt)
{
    std::string line;
    this->log(prompt);

    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        getline(cin, line);
    #endif

    #ifdef ESP32
        while (!Serial.available())
            delay(10);
            
        String str = Serial.readString();
        line = std::string(str.c_str());
    #endif

    return line;
}

std::string Console::input(void)
{
    return this->input("");
}

Console console;
