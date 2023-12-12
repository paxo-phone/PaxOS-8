#include "back.hpp"

#include "gui_class.hpp"
#include "box.hpp"
#include "image.hpp"

Back::Back() : Box(4, 3, 44, 44)
{
    enabledBackground=false;
    
    setBackgroundColor(COLOR_LIGHT);
    setRadius(0);
    setBorderSize(0);

    Image* img = new Image("system/return.png", 15, 11);
    img->load();
    addChild(img);
}