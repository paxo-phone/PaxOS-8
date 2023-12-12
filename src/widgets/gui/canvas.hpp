#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "gui_class.hpp"

class Canvas : public Gui
{
    public:
    GUI_TYPE getType();
    Canvas();
    Canvas(int16_t x, int16_t y, int16_t width, int16_t height);
    void draw();
    void push();
};

#endif
