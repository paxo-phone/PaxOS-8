#ifndef TIME_HPP
#define TIME_HPP

#include "../includes.h"

#ifdef BUILD_EMU

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

#endif /* BUILD_EMU */

#endif /* TIME_HPP */