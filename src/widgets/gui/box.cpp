#include "box.hpp"

GUI_TYPE Box::getType(){return BOX_TYPE;}

Box::Box() = default;

Box::Box(const int16_t x, int16_t const y, const int16_t width, const int16_t height) : Gui(x, y, width, height)
{
    setBorderSize(0);
    setRadius(0);
    setBackgroundColor(COLOR_EXTRA_LIGHT);
    setBorderColor(COLOR_PRIMARY);
    enabledBackground=false;
}

void Box::draw()
{
    if(enabledBackground)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), getRadius(), getBorderSize(), getBackgroundColor(), getBorderColor());
}