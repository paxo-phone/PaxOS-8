#ifndef COLOR_HPP
#define COLOR_HPP

#include "../interface/screen.hpp"

typedef uint16_t color_t;

#define COLOR_BLACK 0x29A6
#define COLOR_GREY 0xA514
#define COLOR_SUCCESS 0x05D2
#define COLOR_PRIMARY 0x6AFA
#define COLOR_ERROR 0xD186
#define COLOR_WARNING 0xE38A
#define COLOR_LIGHT 0xFFFF
#define COLOR_EXTRA_LIGHT 0xF79E
#define COLOR_BLUE 0x03FF
#define COLOR_BEIGE 0xEF5B

#define ALPHA_8B 254
#define ALPHA_16B 0xF7BE

/*###############################################*/
/*             alignement configuration          */
/*###############################################*/

#define LEFT_ALIGNMENT 0
#define RIGHT_ALIGNMENT 1
#define CENTER_ALIGNMENT 2
#define UP_ALIGNMENT 0
#define DOWN_ALIGNMENT 1

/*###############################################*/
/*             graphics configuration            */
/*###############################################*/

#define DEFAULT_BORDER_SIZE 1
#define DEFAULT_H_ALIGNEMENT LEFT_ALIGNMENT
#define DEFAULT_V_ALIGNEMENT UP_ALIGNMENT
#define DEFAULT_RADIUS 10

#define CONTROL_BAR_SIZE 25

extern color_t contrastColor(color_t color);

extern color_t darkenColor(color_t color);

extern color_t exposureColor(color_t color, float exposure); // in %

#endif
