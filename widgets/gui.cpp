#ifndef GUI_CPP
#define GUI_CPP

#include "gui.hpp"

void Gui::init(int16_t x, int16_t y, int16_t width, int16_t height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    autoX = false;
    autoY = false;
    autoW = false;
    autoH = false;

    if(x == AUTO)
        autoX = true;
    if(y == AUTO)
        autoY = true;
    if(width == AUTO)
        autoW = true;
    if(height == AUTO)
        autoH = true;

    lockedSlide = false;

    onclick = nullptr;
    onlongclick = nullptr;
    onreleased = nullptr;
    onscroll = nullptr;

    appCallback = nullptr;
    dataCallback = nullptr;

    verticalSlide = false;
    horizontalSlide = false;

    verticalSlide=false;
    horizontalSlide=false;

    statePress = 0;
    isTouchedState = false;
    enabledBackground = true;

    rendered = false;

    marginX = marginY = 0;
    scroolX = scroolY = 0;

    inercie = 0;
    
    enabled = true;

    inercie = 0;
    
    color = COLOR_BLACK;
    backgroundColor = COLOR_LIGHT;
    borderColor = COLOR_GREY;

    borderSize = DEFAULT_BORDER_SIZE;
    radius = DEFAULT_RADIUS;

    H_alignment = DEFAULT_H_ALIGNEMENT;
    V_alignment = DEFAULT_V_ALIGNEMENT;

    timerPress = 0;

    autoSize = true;
    enabled = true;

    enabledBackground = true;

    parent = nullptr;

    reloadWidget();
}

Gui::~Gui()
{
    for(int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
        {
            delete children[i];
        }
    }
}

void Gui::initScreen()
{
    #ifdef BUILD_PAXO
        uint16_t calibrationData[] = {370, 3950, 273, 239, 3848, 3949, 3872, 302};
        tft_root.setTouchCalibrate(calibrationData);

        pinMode(25, OUTPUT); // power on
        digitalWrite(25, 1);
        pinMode(14, OUTPUT); // 14 for new and 22 for old
        digitalWrite(14, 1);
    #endif
    
    tft_root.init();
}

void Gui::renderAll()
{
    if(parent!=nullptr)
    {
        if(autoX)
            setX(0);
        if(autoY)
        {
            if(parent->children[0] == this)
                setY(0);
            else
            {
                uint i = 0;
                for(; i<parent->children.size() && parent->children[i] != this; i++);
                setY(parent->children[i-1]->getY() + parent->children[i-1]->getHeight() + 10);
            }
        }
        
        if(autoW)
            width=parent->getWidth()-getRelativeX()*2;

        updateSizes();
    }

    if(!isEnabled()) // don't render if the object is disabled
        return;

    if(upFromDrawAll==nullptr) // if it's the first call of drawAll, define himself as the root
        upFromDrawAll=this;
    
    if(!rendered)   // render if not already rendered
    {
        l_tft.deleteSprite();
        l_tft.setPsram(true);
        l_tft.setColorDepth(16);
        l_tft.createSprite(this->getWidth(), this->getHeight());

        if(parent == nullptr) // no parent -> background = white
        {
            l_tft.fillSprite(COLOR_LIGHT);
        }else // has parent -> background = parent background
        {
            if(!enabledBackground)
                setBackgroundColor(parent->getBackgroundColor());
            
            l_tft.fillSprite(parent->getBackgroundColor());
        }

        draw(); // draw himself in a sprite
        
        for (int i = 0; i < children.size(); i++) // render every children
        {
            if(children[i] != nullptr)
                children[i]->renderAll();
        }

        afterRender();

        rendered = true;
    }

    if(upFromDrawAll==this)
    {
        upFromDrawAll=nullptr;

        if(parent != nullptr)
            l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY(), parent->getBackgroundColor());
        else
            l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY(), ALPHA_16B);
    }
    else
    {
        l_tft.pushSprite(&parent->l_tft, getRelativeX(), getRelativeY(), ALPHA_16B);
    }
}

bool Gui::updateAll()
{
    virtual_update();
    if(parent == nullptr) // automatically update events
    {
        eventHandler.update();
        touch.update();
    }
    if(rendered == false)
    {
        renderAll();
    }
    if(!isEnabled())
        return false;

    bool children_updated = false;
    
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->updateAll()) 
                children_updated=true;
    }

    if(children_updated || update())
    {
        return true;
    }
    
    return false;
}

bool Gui::update()
{
    if (widgetPressed != nullptr && widgetPressed != this)
        return false;
    
    bool focused = isFocuced();
    bool screenTouched = touch.isTouch();

    bool returnValue = false;

    if (screenTouched && focused) // this object is touched
    {
        if (widgetPressed == nullptr) // first object pressed
        {
            widgetPressed = this;
            objectPressState = PRESSED;
            
            timerPress=millis(); // reset timer

            ClickEffect();

            EventOnClick();

            if(onclick!=nullptr)
            {
                onclick(appCallback, this, dataCallback);
                returnValue = true;
            }
        }
        else// already pressed
        {
            if(objectPressState != PRESSED && timerPress+LONG_PRESS_TIME<millis())
            {
                EventOnLongClick();

                if(onlongclick!=nullptr)
                {
                    onlongclick(appCallback, this, dataCallback);
                    
                    return true;
                }

                objectPressState = RELEASED;
            }

            if(touch.stateSlider) // if slide
            {
                objectPressState = SLIDED;

                if(onscroll!=nullptr)
                {
                    onscroll(appCallback, this, dataCallback);
                    return true;
                }

                if (!horizontalSlide && !verticalSlide && parent != nullptr)
                {
                    widgetPressed = parent;

                    parent->objectPressState = SLIDED;
                    this->objectPressState = NOT_PRESSED;

                    this->isTouchedState = false;
                    parent->isTouchedState = true;

                    ReleasedEffect();
                }

                EventOnScroll();
            }
        }
    }

    if (screenTouched && !focused) // touch point had slided without releasing
    {
        if(touch.stateSlider) // if slide
        {
            objectPressState = SLIDED;

            if(onscroll!=nullptr)
            {
                onscroll(appCallback, this, dataCallback);
                return true;
            }

            if (!horizontalSlide && !verticalSlide && parent != nullptr)
            {
                widgetPressed = parent;

                parent->objectPressState = SLIDED;
                this->objectPressState = NOT_PRESSED;

                this->isTouchedState = false;
                parent->isTouchedState = true;

                ReleasedEffect();
            }

            EventOnScroll();
        }
    }

    if (!screenTouched && objectPressState != NOT_PRESSED) // screen released
    {
        widgetPressed = nullptr;
        ReleasedEffect();

        if (objectPressState == PRESSED)
        {
            isTouchedState = true;

            EventOnReleased();

            //print ("released");

            if(onreleased!=nullptr)
            {
                onreleased(appCallback, this, dataCallback);
                return true;
            }
        }

        objectPressState = NOT_PRESSED;
    }

    return false;
}

void Gui::reload()
{
    renderAll();
}

bool Gui::isFocuced()
{
    return Touched(getAbsoluteX(), getAbsoluteY(), width, height); 
}

bool Gui::isTouched()
{
    if(isTouchedState && !isFocuced())
    {
        isTouchedState = false;
        return true;
    }
    return 0;
}

void Gui::reloadWidget()
{
    if(parent!=nullptr)
        parent->reloadWidget();
    rendered=false;
}

int16_t Gui::getX()
{
    return this->x;
}

int16_t Gui::getY()
{
    return this->y;
}

int16_t Gui::getWidth()
{
    return this->width;
}

int16_t Gui::getHeight()
{
    return this->height;
}

int16_t Gui::getAbsoluteX()
{
    if(parent==nullptr)
        return getRelativeX();
    else
        return getRelativeX() + parent->getAbsoluteX();
}

int16_t Gui::getAbsoluteY()
{
    if(parent==nullptr)
        return getRelativeY();
    else
        return getRelativeY() + parent->getAbsoluteY();
}

int16_t Gui::getAbsoluteFixX()
{
    if(parent==nullptr)
        return getRelativeFixX();
    else
        return getRelativeFixX() + parent->getAbsoluteFixX();
}

int16_t Gui::getAbsoluteFixY()
{
    if(parent==nullptr)
        return getRelativeFixY();
    else
        return getRelativeFixY() + parent->getAbsoluteFixY();
}

int16_t Gui::getRelativeX()
{
    return this->getX() + ((parent!=nullptr && !noMargin) ? (parent->getBorderSize() + parent->getMarginX() + parent->getRadius()/2 + parent->scroolX) : (0));
}

int16_t Gui::getRelativeY()
{
    return this->getY() + ((parent!=nullptr && !noMargin) ? (parent->getBorderSize() + parent->getMarginY() + parent->getRadius()/2 + parent->scroolY) : (0));
}

int16_t Gui::getRelativeFixX()
{
    return this->getX() + ((parent!=nullptr && !noMargin) ? (parent->getBorderSize() + parent->getMarginX() + parent->getRadius()/2) : (0));
}

int16_t Gui::getRelativeFixY()
{
    return this->getY() + ((parent!=nullptr && !noMargin) ? (parent->getBorderSize() + parent->getMarginY() + parent->getRadius()/2) : (0));
}

void Gui::setX(int16_t x)
{
    this->x=x;
    reloadWidget();
}                               

void Gui::setY(int16_t y)
{
    this->y=y;
    reloadWidget();
}

void Gui::setWidth(int16_t width)
{
    this->width=width;
    reloadWidget();
}

void Gui::setHeight(int16_t height)
{
    this->height=height;
    reloadWidget();
}

void Gui::setMarginX(int16_t marginX)
{
    this->marginX = marginX;
    reloadWidget();
}

void Gui::setMarginY(int16_t marginY)
{
    this->marginY = marginY;
    reloadWidget();
}

int16_t Gui::getMarginX()
{
    return this->marginX;
}

int16_t Gui::getMarginY()
{
    return this->marginY;
}

void Gui::addChild(Gui *child)
{
    if(child == nullptr)
        return;
    children.push_back(child);
    child->setParent(this);

    reloadWidget();
}

void Gui::removechildren()
{
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            delete children[i];
    }
    for (; children.size();)
    {
        children.erase(children.begin());
    }
}

void Gui::setParent(Gui *parent)
{
    this->parent = parent;
    reloadWidget();
}

Gui* Gui::getMaster()
{
    if(parent == nullptr)
        return this;
    else
        return parent->getMaster();
}

void Gui::setColor(color_t color)
{
    this->color=color;
    reloadWidget();
}

void Gui::setBackgroundColor(color_t backgroundColor)
{
    this->backgroundColor=backgroundColor;
    reloadWidget();
}

void Gui::setBorderColor(color_t borderColor)
{
    this->borderColor=borderColor;
    reloadWidget();
}

void Gui::setBorderSize(int16_t borderSize)
{
    this->borderSize=borderSize;
    reloadWidget();
}

color_t Gui::getColor()
{
    return this->color;
}

color_t Gui::getBackgroundColor()
{
    return this->backgroundColor;
}

color_t Gui::getBorderColor()
{
    return this->borderColor;
}

int16_t Gui::getBorderSize()
{
    return this->borderSize;
}

void Gui::setHorizontalAlignment(Alignment alignment)
{
    this->H_alignment=alignment;
    reloadWidget();
}

void Gui::setVerticalAlignment(Alignment alignment)
{
    this->V_alignment=alignment;
    reloadWidget();
}

Alignment Gui::getHorizontalAlignment()
{
    return this->H_alignment;
}

Alignment Gui::getVerticalAlignment()
{
    return this->V_alignment;
}

void Gui::setRadius(uint16_t radius)
{
    this->radius=radius;
    reloadWidget();
}

int16_t Gui::getLowestX()
{
    int16_t x = this->getAbsoluteX()+getWidth();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestX()+children[i]->getWidth()>x)
                x=children[i]->getLowestX()+children[i]->getWidth();
    }
    return x;
}

int16_t Gui::getHighestX()
{
    int16_t x = this->getAbsoluteX();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestX()<x)
                x=children[i]->getLowestX();
    }
    return x;
}

int16_t Gui::getLowestY()
{
    int16_t y = getAbsoluteFixY() + getHeight();

    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestY()>y)
                y=children[i]->getLowestY();
    }

    /*if (children.size() == 0)
        print ("   " + to_string(y));
    else
        print (" " + to_string(y));*/
    return y;
}

int16_t Gui::getHighestY()
{
    int16_t y = this->getAbsoluteY();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestY()<y)
                y=children[i]->getLowestY();
    }
    return y;
}

void Gui::setTheme(uint8_t theme)
{
    color = theme_color[theme][0];
    backgroundColor = theme_color[theme][1];
    borderColor = theme_color[theme][2];
    reloadWidget();
}

Gui* Gui::getParent()
{
    return this->parent; // warning, can return nullptr
}

void Gui::EventOnClick()
{
    return; // do nothing
}

void Gui::EventOnLongClick()
{
    return; // do nothing
}

void Gui::EventOnReleased()
{
    return; // do nothing
}

bool Gui::EventOnScroll()
{
    int16_t mvtX=touch.isSlidingHorizontally();
    int16_t mvtY=touch.isSlidingVertically();

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        reloadWidget(); // a optimiser
    
    if(mvtX && horizontalSlide)
    {
        this->scroolX-=mvtX;
        touch.resetScrollHorizontal();
    }
    if(mvtY && verticalSlide && getLowestY() > -(this->scroolY-mvtY) + this->height  && 0 < -(this->scroolY-mvtY))
    {
        this->scroolY-=mvtY;
        touch.resetScrollVertical();
    }

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        renderAll();
    return mvtX || mvtY;
}


#endif