#ifndef CANVAS_HPP
#define CANVAS_HPP

class Canvas : public Gui
{
    public:
    GUI_TYPE getType(){return CANVAS_TYPE;}
    Canvas(){};
    Canvas(int16_t x, int16_t y, int16_t width, int16_t height);
    void draw();
    void free(){}
};

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

void Canvas::draw()
{
    
}

#endif