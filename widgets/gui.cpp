#ifndef GUI_CPP
#define GUI_CPP

#include "gui.hpp"

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

        pinMode(25, OUTPUT);
        digitalWrite(25, 1);
        pinMode(14, OUTPUT);
        digitalWrite(14, 1);
    #endif
    
    tft_root.init();
}

void Gui::init(int16_t x, int16_t y, int16_t width, int16_t height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    onclick = nullptr;
    onlongclick = nullptr;
    onreleased = nullptr;
    onscroll = nullptr;

    reloadWidget();
}

bool Gui::isFocuced()
{
    return Touched(getAbsoluteX(), getAbsoluteY(), width, height); 
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
        reloadWidget();
    
    if(mvtX && horizontalSlide)
    {
        this->scroolX-=mvtX;
        touch.resetScrollHorizontal();
    }
    if(mvtY && verticalSlide)
    {
        this->scroolY-=mvtY;
        touch.resetScrollVertical();
    }

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        renderAll();
    return mvtX || mvtY;
}

bool Gui::isTouched()
{
    if(statePress && !isFocuced())
    {
        statePress = false;
        return true;
    }
    return 0;
}

bool Gui::update()
{
    bool s = isFocuced();

    if(s)
    {
        if(statePress==0 && lockedSlide==0)
        {
            statePress=1;
            timerPress=millis();
            EventOnClick();
            if(onclick!=nullptr)
            {
                onclick(appCallback, this, dataCallback);
                return true;
            }
        }
        if(statePress==1)
        {
            if(timerPress+LONG_PRESS_TIME<millis())
            {
                EventOnLongClick();
                if(onlongclick!=nullptr)
                {
                    onlongclick(appCallback, this, dataCallback);
                    statePress=2;
                    return true;
                }
            }
        }
        if(touch.stateSlider)
        {
            if(EventOnScroll())
            {
                statePress=0;
                lockedSlide=1;
            }
            if(onscroll!=nullptr)
            {
                onscroll(appCallback, this, dataCallback);
                return true;
            }
            
        }
    }else
    if(statePress!=0)
    {
        if(lockedSlide==0)
        {
            EventOnReleased();
            if(onreleased!=nullptr)
            {
                statePress=0;
                onreleased(appCallback, this, dataCallback);
                return true;
            }
        }
    }
    if(!s)
    {
        lockedSlide=0;
    }
    return false;
}

void Gui::renderAll()
{
    if(parent!=nullptr)
    {
        if(getX()==AUTO)
            setX(0);
        if(getY()==AUTO)
        {
            if(parent->children[0] == this)
                setY(0);
            else
            {
                uint i = 0;
                for(; i<parent->children.size() && parent->children[i] != this; i++);
                setY(parent->children[i-1]->getY() + parent->children[i-1]->getHeight());
            }
        }
        
        if(width==AUTO)
            width=parent->getWidth()-getRelativeX()*2;

        updateSizes();
        
        if(height==AUTO_FULL)
            width=parent->getHeight()-getRelativeY()*2;
        print(std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(width) + " " + std::to_string(height));
    }

    if(!isEnabled())
        return;

    if(upFromDrawAll==nullptr)
        upFromDrawAll=this;
    
    if(!rendered)
    {
        l_tft.deleteSprite();
        l_tft.setPsram(true);
        l_tft.setColorDepth(16);
        l_tft.createSprite(this->getWidth(), this->getHeight());

        if(upFromDrawAll==this)
            l_tft.fillSprite(0xFFFF);
        else if(!enabledBackground)
        {
            backgroundColor = parent->getBackgroundColor();
            l_tft.fillSprite(getBackgroundColor());
        }
        

        draw();
        
        for (int i = 0; i < children.size(); i++)
        {
            if(children[i] != nullptr)
                children[i]->renderAll();
        }
    }

    if(upFromDrawAll==this)
    {
        upFromDrawAll=nullptr;

        l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY());
    }
    else
    {
        l_tft.pushSprite(&parent->l_tft, getRelativeX(), getRelativeY(), getBackgroundColor());
    }
}

void Gui::reloadWidget()
{
    if(parent!=nullptr)
        parent->reloadWidget();
    rendered=false;
}

bool Gui::updateAll()
{
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
    rendered=false;
}

Gui* Gui::getMaster()
{
    if(parent == nullptr)
        return this;
    else
        return parent->getMaster();
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

void Gui::setMarginX(int16_t marginX)
{
    this->marginX = marginX;
}

void Gui::setMarginY(int16_t marginY)
{
    this->marginY = marginY;
}

int16_t Gui::getMarginX()
{
    return this->marginX;
}

int16_t Gui::getMarginY()
{
    return this->marginY;
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
        return getRelativeY() + parent->getAbsoluteY() + parent->getBorderSize() + parent->getRadius()/2;
}

int16_t Gui::getRelativeX()
{
    return this->getX() + ((parent!=nullptr) ? (parent->getBorderSize() + parent->getRadius()/2) : (0));
}

int16_t Gui::getRelativeY()
{
        return this->getY() + ((parent!=nullptr) ? (parent->getBorderSize() + parent->getRadius()/2) : (0));
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

Alignment Gui::getAlignment()
{
    return this->H_alignment;
}

void Gui::setX(int16_t x)
{
    this->x=x;
    rendered=false;
}

void Gui::setY(int16_t y)
{
    this->y=y;
    rendered=false;
}

void Gui::setWidth(int16_t width)
{
    this->width=width;
    rendered=false;
}

void Gui::setHeight(int16_t height)
{
    this->height=height;
    rendered=false;
}

void Gui::setColor(color_t color)
{
    this->color=color;
    rendered=false;
}

void Gui::setBackgroundColor(color_t backgroundColor)
{
    this->backgroundColor=backgroundColor;
    rendered=false;
}

void Gui::setBorderColor(color_t borderColor)
{
    this->borderColor=borderColor;
    rendered=false;
}

void Gui::setBorderSize(int16_t borderSize)
{
    this->borderSize=borderSize;
    rendered=false;
}

void Gui::setTheme(uint8_t theme)
{
    color = theme_color[theme][0];
    backgroundColor = theme_color[theme][1];
    borderColor = theme_color[theme][2];
    rendered=false;
}

void Gui::setHorizontalAlignment(Alignment alignment)
{
    this->H_alignment=alignment;
    rendered=false;
}

void Gui::setVerticalAlignment(Alignment alignment)
{
    this->V_alignment=alignment;
    rendered=false;
}

void Gui::setRadius(uint16_t radius)
{
    this->radius=radius;
    rendered=false;
}

int16_t Gui::getLowestY()
{
    int16_t y = this->getAbsoluteY()+getHeight();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestY()+children[i]->getHeight()>y)
                y=children[i]->getLowestY()+children[i]->getHeight();
    }
    return y;
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

Gui* Gui::getParent()
{
    return this->parent; // warning, can return nullptr
}

void Gui::reload()
{
    renderAll();
}

#endif