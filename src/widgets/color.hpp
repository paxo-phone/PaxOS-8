#ifndef COLOR_HPP
#define COLOR_HPP

#include "../interface/screen.hpp"

typedef uint16_t color_t;

enum : color_t
{
    COLOR_BLACK = 0x29A6,
    COLOR_GREY = 0xA514,
    COLOR_SUCCESS = 0x05D2,
    COLOR_PRIMARY = 0x6AFA,
    COLOR_ERROR = 0xD186,
    COLOR_WARNING = 0xE38A,
    COLOR_LIGHT = 0xFFFF,
    COLOR_EXTRA_LIGHT = 0xF79E,
    COLOR_BLUE = 0x03FF,
    COLOR_BEIGE = 0xEF5B
};

#define ALPHA_8B 254
#define ALPHA_16B 0xF7BE

color_t rgb(uint8_t r, uint8_t g, uint8_t b);

color_t contrastColor(color_t color);
color_t darkenColor(color_t color);
color_t exposureColor(color_t color, float exposure);

#endif
