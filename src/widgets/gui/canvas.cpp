#include "canvas.hpp"

GUI_TYPE Canvas::getType(){return CANVAS_TYPE;}

Canvas::Canvas(){}

Canvas::Canvas(int16_t x, int16_t y, int16_t width, int16_t height) : Gui(x, y, width, height)
{
    setBorderSize(0);
    setRadius(0);

    l_tft.deleteSprite();
    l_tft.setPsram(true);
    l_tft.setColorDepth(16);
    l_tft.createSprite(this->getWidth(), this->getHeight());
    l_tft.fillSprite(COLOR_LIGHT);
}

void Canvas::draw(){}
