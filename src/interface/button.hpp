#ifndef HOME_BUTTON_HPP
#define HOME_BUTTON_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

namespace home_button
{
    /**
     * @brief Initialise le bouton home
    */
    void init(void);

    /**
     * @brief Vérifie si le bouton home est pressé
     * @return Vrai si le bouton home est pressé, faux sinon
    */
    bool isPressed(void);
}

#endif /* HOME_BUTTON_HPP */
