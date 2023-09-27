#ifndef TOUCH_MANAGER
#define TOUCH_MANAGER

#define SLIDE_UP 1
#define SLIDE_DOWN 2
#define SLIDE_RIGHT 3
#define SLIDE_LEFT 4

#define FLUIDITY 1
#define FLUIDITY_START 20

#define timeToWait 10

class TouchManager
{
    public:
    uint16_t getX();
    uint16_t getY();
    bool isTouch();

    int16_t isSlidingHorizontally(bool force = false); // sliding right
    int16_t isSlidingVertically(bool force = false);   // sliding up

    void resetScrollVertical();
    void resetScrollHorizontal();
   
    void update();
    void calibrate();
    
    uint8_t stateSlider=0;

    private:
    bool isTouchRead();
    bool validTouch();
    
    uint16_t tx, ty;
    bool stateTouch;
    double lastClick = millis();
    bool isPreviousTouch = 0;
    unsigned long timerSlider;
    
    uint16_t fx, fy = 0;
    unsigned long timerUpdate = millis();
};

TouchManager touch;

class touchZone
{
    public:
    touchZone(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        this->x=x;
        this->y=y;
        this->width=width;
        this->height=height;
    }
    bool isTouch()
    {
        uint16_t xx = touch.getX();
        uint16_t yy = touch.getY();
        return x<xx && xx<x+width && y<yy && yy<y+height;
    }
    private:
    uint16_t x, y, width, height;
};

bool Touched(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif // TOUCH_MANAGER