#ifndef BOX_HPP
#define BOX_HPP

class Box : public Gui
{
    public:
    GUI_TYPE getType(){return BOX_TYPE;}
    Box(int16_t x, int16_t y, int16_t width, int16_t height);
    void draw();
    void free(){}
};

Box::Box(int16_t x, int16_t y, int16_t width, int16_t height)
{
    init(x, y, width, height);
    setBorderSize(0);
    setRadius(2);
    setBackgroundColor(COLOR_EXTRA_LIGHT);
    setBorderColor(COLOR_PRIMARY);
    enabledBackground=true;
}

void Box::draw()
{
    if(enabledBackground)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), getRadius(), getBorderSize(), getBackgroundColor(), getBorderColor());
}

#endif