#ifndef BUTTON_HPP
#define BUTTON_HPP

//#include "../gui.hpp"
#include "label.hpp"

class Button : public Label
{
    public:
    GUI_TYPE getType(){return BUTTON_TYPE;}

    Button();

    Button(int16_t x, int16_t y, int16_t width, int16_t height, std::string text = "");
    void ClickEffect();
    void ReleasedEffect();

    color_t oldBackgroundColor;
};

#endif