#ifndef TIME_HPP
#define TIME_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include <chrono>
#include <cstdint>

/**
  * @brief Retourne le nombre de microsecondes depuis une date fixée
  * @return Entier représentant le nombre de microsecondes depuis une date fixée
  * @remark Cette fonction est déjà présente sur l'esp32
  */
uint64_t micros(void);

/**
  * @brief Retourne le nombre de millisecondes depuis une date fixée
  * @return Entier représentant le nombre de microsecondes depuis une date fixée
  * @remark Cette fonction est déjà présente sur l'esp32
  */
uint64_t millis(void);

/**
 * @brief Attend un certain nombre de millisecondes
 * @param ms Entier représentant le nombre de millisecondes à attendre
 * @remark Il s'agit d'unen fonction bloquante
*/
void delay(uint64_t ms);

#endif /* #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) */

#endif /* TIME_HPP */