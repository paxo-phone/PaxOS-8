#include "button.hpp"

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

namespace home_button
{
    #define HOME_BUTTON_PIN 33 // 32 pour l'ancien modèle

    void init(void)
    {
        #ifdef ESP32
            pinMode(HOME_BUTTON_PIN, INPUT_PULLUP);
        #endif
    }

    bool isPressed(void)
    {
        #ifdef ESP32
            return (HIGH == digitalRead(HOME_BUTTON_PIN));
        #endif

        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            /*
                Peut être mettre en place un event handler
                avec sdl pour pouvoir utiliser le home button
                sur l'émulateur
            */  
            return false;
        #endif

        return false;
    }

}
