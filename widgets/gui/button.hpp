#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "label.hpp"

class Button : public Label
{
    public:
    GUI_TYPE getType(){return BUTTON_TYPE;}

    Button()
    {
        init(0, 0, 0, 0);
        this->fontHeight = 20;
        this->bold = true;
        enabledBackground=true;
        
        setBackgroundColor(COLOR_PRIMARY);
        setTextColor(COLOR_LIGHT);
        setRadius(10);
        setHorizontalAlignment(CENTER_ALIGNMENT);
        setVerticalAlignment(CENTER_ALIGNMENT);
        setBorderSize(0);
    }

    Button(int16_t x, int16_t y, int16_t width, int16_t height, std::string text)
    {
        init(x, y, width, height);
        this->text = text;
        this->fontHeight = 20;
        this->bold = true;
        enabledBackground=true;
        
        setBackgroundColor(COLOR_PRIMARY);
        setTextColor(COLOR_LIGHT);
        setRadius(10);
        setHorizontalAlignment(CENTER_ALIGNMENT);
        setVerticalAlignment(CENTER_ALIGNMENT);
        setBorderSize(0);
    }

    void ClickEffect()
    {
        oldBackgroundColor = backgroundColor;
        backgroundColor = darkenColor(backgroundColor);
        rendered = false;
        renderAll();
    }
    void ReleasedEffect()
    {
        backgroundColor = oldBackgroundColor;
        rendered = false;
        renderAll();
    }

    color_t oldBackgroundColor;
};

#endif