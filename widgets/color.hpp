#ifndef COLOR_HPP
#define COLOR_HPP

#include "gui.hpp"

/*###############################################*/
/*              theme configuration              */
/*###############################################*/

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

#define ALPHA_8B 0xEF5D
#define ALPHA_16B 1

#define THEME_BLACK 0
#define THEME_BLUE 1
#define THEME_GREEN 2
#define THEME_RED 3
#define THEME_YELLOW 4
#define THEME_WHITE 5
#define THEME_CONTRAST 6    // hight constrast

color_t theme_color[][3]=
{   // color, background color, border color
    {COLOR_LIGHT, COLOR_BLACK, COLOR_LIGHT},   // black
    {COLOR_LIGHT, 0x22B5, 0x4800},   // blue
    {COLOR_LIGHT, 0x3C42, 0x4800},   // green
    {COLOR_LIGHT, 0xC861, 0x4800},   // red
    {COLOR_BLACK, 0xFFC8, 0xEF37},   // yellow
    {COLOR_BLACK, COLOR_LIGHT, 0xBDF7},   // white
    {COLOR_BLACK, COLOR_LIGHT, COLOR_BLACK}    // contrast
};

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

#define DEFAULT_THEME THEME_CONTRAST
#define DEFAULT_BORDER_SIZE 1
#define DEFAULT_H_ALIGNEMENT LEFT_ALIGNMENT
#define DEFAULT_V_ALIGNEMENT UP_ALIGNMENT
#define DEFAULT_RADIUS 2

#define CONTROL_BAR_SIZE 25

color_t contrastColor(color_t color)
{
    uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;

    int f = (r+g+b)/3;
    if(f>130)
    {
        r = 0;
        g = 0;
        b = 0;
    }else
    {
        r = 255;
        g = 255;
        b = 255;
    }
    return tft_root.color565(r, g, b);
}

color_t darkenColor(color_t color)
{
    uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;
    return tft_root.color565(r*2/3, g*2/3, b*2/3);
}

color_t exposureColor(color_t color, float exposure) // in %
{
    uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;
    r=255-r; g=255-g; b=255-b;
    r=r*exposure/100;
    g=g*exposure/100;
    b=b*exposure/100;
    r=255-r; g=255-g; b=255-b;

    return tft_root.color565(r, g, b);
}

#endif