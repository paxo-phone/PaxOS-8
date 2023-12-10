#ifndef BOX_HPP
#define BOX_HPP

#include "../gui_class.hpp"

class Box : public Gui
{
    public:
    GUI_TYPE getType();
    Box();
    Box(int16_t x, int16_t y, int16_t width, int16_t height);
    void draw();
};

#endif
