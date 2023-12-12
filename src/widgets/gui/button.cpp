#ifndef BUTTON_CPP
#define BUTTON_CPP

#include "gui_class.hpp"
#include "button.hpp"
#include "label.hpp"

Button::Button() : Label()
{
    this->setFontSize(20);
    this->setBold(true);
    enabledBackground=true;
    
    setBackgroundColor(COLOR_BLACK);
    setTextColor(COLOR_LIGHT);
    setRadius(19);
    setHorizontalAlignment(CENTER_ALIGNMENT);
    setVerticalAlignment(CENTER_ALIGNMENT);
    setBorderSize(0);
}

Button::Button(int16_t x, int16_t y, int16_t width, int16_t height, std::string text) : Label(x, y, width, height, text)
{
    this->setFontSize(20);
    this->setBold(true);
    enabledBackground=true;
    
    setBackgroundColor(COLOR_BLACK);
    setTextColor(COLOR_LIGHT);
    setRadius(19);
    setHorizontalAlignment(CENTER_ALIGNMENT);
    setVerticalAlignment(CENTER_ALIGNMENT);
    setBorderSize(0);
}

void Button::ClickEffect()
{
    oldBackgroundColor = backgroundColor;
    backgroundColor = darkenColor(backgroundColor);
    reloadAlone();
    renderAll();
}
void Button::ReleasedEffect()
{
    backgroundColor = oldBackgroundColor;
    reloadAlone();
    renderAll();
}

#endif