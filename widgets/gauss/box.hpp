#ifndef BOX_HPP
#define BOX_HPP

class Box : public Gauss
{
    public:
    GAUSS_TYPE getType(){return BOX_TYPE;}
    Box(int16_t x, int16_t y, int16_t width, int16_t height);
    void draw();
    void free(){}
};

Box::Box(int16_t x, int16_t y, int16_t width, int16_t height)
{
    init(x, y, width, height);
    setRadius(0);
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setPaddingY(0);
    setMarginY(0);
    setBorderSize(0);
    setBackgroundColor(COLOR_LIGHT);
    enabledBackground=true;
}

void Box::draw()
{
    if(enabledBackground)
        l_tft.fillRect(0, 0, getWidth(), getHeight(), getBackgroundColor());
}

#endif