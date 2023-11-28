#include "button.hpp"

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include "../tasks/tasks.hpp"

namespace home_button
{
    #define HOME_BUTTON_PIN 33 // 32 pour l'ancien modèle

    #define RELEASED 0
    #define PRESSED  1

    uint8_t status = RELEASED;

    void init(void)
    {
        #ifdef ESP32
            pinMode(HOME_BUTTON_PIN, INPUT_PULLUP);
        #endif
        setInterval(new Callback<void>(&update), 10);
    }

    void update(void)
    {
        #ifdef ESP32
            status = (digitalRead(HOME_BUTTON_PIN) == HIGH);
        #endif

        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            /*
                Peut être mettre en place un event handler
                avec sdl pour pouvoir utiliser le home button
                sur l'émulateur
            */  
            status = RELEASED;
        #endif
    }

    bool isPressed(void)
    {
        return status;
    }

}
