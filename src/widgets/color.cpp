#include "color.hpp"

color_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return lgfx::color565(r, g, b);
}

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
    return lgfx::color565(r, g, b);
}

color_t darkenColor(color_t color)
{
    uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;
    return lgfx::color565(r*2/3, g*2/3, b*2/3);
}

color_t exposureColor(color_t color, float exposure)
{
    uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;
    r=255-r; g=255-g; b=255-b;
    r=r*exposure/100;
    g=g*exposure/100;
    b=b*exposure/100;
    r=255-r; g=255-g; b=255-b;

    return lgfx::color565(r, g, b);
}
