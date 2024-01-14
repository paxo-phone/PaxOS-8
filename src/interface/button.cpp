#include "button.hpp"
#include <LovyanGFX.hpp>

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include "../tasks/tasks.hpp"

namespace home_button
{
    #define HOME_BUTTON_PIN 33 // 32 pour l'ancien modèle
    enum {
        NOT_PRESSED,
        PRESSED,
        RELEASED
    };

    uint8_t status = NOT_PRESSED;

    void init(void)
    {
        #ifdef ESP32
            pinMode(HOME_BUTTON_PIN, INPUT_PULLUP);
        #endif
        setInterval(new Callback<void>(&update), 10);
    }

    void update(void)
    {
        if(status == RELEASED)
            return;
        
        bool pressed = false;
        #ifdef ESP32
            pressed = !(digitalRead(HOME_BUTTON_PIN) == HIGH); // HIGH corresponds au bouton relaché (PULLUP)
        #endif

        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            pressed = lgfx::gpio_in(HOME_BUTTON_PIN);
        #endif

        if (!pressed && status == PRESSED)
            status = RELEASED;
            
        if(pressed && status == NOT_PRESSED)
            status = PRESSED;
    }

    bool isPressed(void)
    {
        bool s = (status == RELEASED);
        if(s)
            status = NOT_PRESSED;
        return s;
    }
}