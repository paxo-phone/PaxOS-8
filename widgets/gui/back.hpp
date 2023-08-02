#ifndef BACK_WIDGET_HPP
#define BACK_WIDGET_HPP

#include "box.hpp"
#include "image.hpp"

class Back : public Box
{
    public:
    Back()
    {
        init(4, 3, 44, 44);
        enabledBackground=false;
        
        setBackgroundColor(COLOR_LIGHT);
        setRadius(0);
        setBorderSize(0);

        Image* img = new Image("system/return.png", 15, 11);
        img->load();
        addChild(img);
    }
};

#endif