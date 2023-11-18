#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../includes.h"

#ifdef BUILD_EMU

#include <cstdint>
#include <cstdlib>

/**
 * @brief Génère aléatoirement un entier compris entre deux bornes 
 * @param start Borne inférieur de l'entier généré
 * @param end Borne supérieur de l'entier généré (exclue)
 * @return Entier généré
*/


int16_t random(const int16_t start, const int16_t end);

#endif /* BUILD_EMU */

#endif /* RANDOM_HPP */