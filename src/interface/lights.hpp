/**
 * @file lights.hpp
 * @brief Fichier d'en tête définissant les fonctions et constantes relatives au contrôle de la DEL.
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif


#include <stdint.h>

namespace light
{
    /**
     * @brief Initialise la DEL
    */
    void init(void);
    
    /**
     * @brief Fixe l'état de la DEL
     * @param state Un entier compris entre -1 (minimum) et 255 (maximum) représentant l'état de la DEL.
    */
    void setAnalogState(int16_t state);

    /**
     * @brief Allume la DEL (équivalent à setAnalogState(255))
    */
    void turnOn(void);

    /**
     * @brief Éteint la DEL (équivalent à setAnalogState(-1))
    */
    void turnOff(void);

    /**
     * @brief Fixe un état binaire (allumé ou éteint) à la DEL. 
     * @param state Une valeur boléenne représentant l'état de la DEL (true = Allumé, false = Éteint)
    */
    void setState(bool state);
}
    
#endif /* LIGHT_HPP */
