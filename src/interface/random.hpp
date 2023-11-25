#ifndef RANDOM_HPP
#define RANDOM_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include <cstdint>
#include <cstdlib>

/**
 * @brief Génère aléatoirement un entier compris entre deux bornes 
 * @param start Borne inférieur de l'entier généré
 * @param end Borne supérieur de l'entier généré (exclue)
 * @return Entier généré
*/


int16_t random(const int16_t start, const int16_t end);

#endif /* #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) */

#endif /* RANDOM_HPP */