#ifndef GAUSS_CPP
#define GAUSS_CPP

#include "gauss.hpp"

Gauss::~Gauss()
{
    for(int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
        {
            delete children[i];
        }
    }
}

void Gauss::initScreen()
{
    tft_root.init();
}

void Gauss::init(int16_t x, int16_t y, int16_t width, int16_t height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setPaddingY(0);
    onclick = nullptr;
    onlongclick = nullptr;
    onreleased = nullptr;
    onscroll = nullptr;
    speed=1;

    reloadWidget();
}

bool Gauss::isFocuced()
{
    return Touched(getAbsoluteX(), getAbsoluteY(), width, height); 
}

void Gauss::EventOnClick()
{
    return; // do nothing
}

void Gauss::EventOnLongClick()
{
    return; // do nothing
}

void Gauss::EventOnReleased()
{
    return; // do nothing
}

bool Gauss::EventOnScroll()
{
    int16_t mvtX=touch.isSlidingHorizontally();
    int16_t mvtY=touch.isSlidingVertically();

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        reloadWidget();
    
    if(mvtX && horizontalSlide)
    {
        this->paddingX-=mvtX;
        touch.resetScrollHorizontal();
    }
    if(mvtY && verticalSlide)
    {
        this->paddingY-=mvtY;
        touch.resetScrollVertical();
    }

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        drawAll();
    return mvtX || mvtY;
}

bool Gauss::isTouched()
{
    if(statePress && !isFocuced())
    {
        statePress = false;
        return true;
    }
    return 0;
}

bool Gauss::update()
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

void Gauss::drawAll(bool draw_)
{
    if(!isEnabled())
        return;

    if(upFromDrawAll==nullptr)
    {
        upFromDrawAll=this;
    }
    
    if(!rendered)
    {
        l_tft.deleteSprite();
        l_tft.setPsram(true);
        l_tft.setColorDepth((speed) ? (8) : (16));
        l_tft.createSprite(this->getWidth(), this->getHeight());

        if(upFromDrawAll==this)
            l_tft.fillSprite(0xFFFF);
        else
            l_tft.fillSprite((speed) ? (ALPHA_8B) : (ALPHA_16B));

        draw();
        
        for (int i = 0; i < children.size(); i++)
        {
            if(children[i] != nullptr)
                children[i]->drawAll();
        }
    }

    if(upFromDrawAll==this)
    {
        upFromDrawAll=nullptr;

        l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY());
    }
    else
    {
        l_tft.pushSprite(&parent->l_tft, getRelativeX(), getRelativeY(), (speed) ? (ALPHA_8B) : (ALPHA_16B));
    }
}

void Gauss::renderAll()
{
    if(!isEnabled())
        return;
    
    if(upFromDrawAll==this)
    {
        reload_afterunlocked=false;
        return;
    }
    
    if(!rendered)
    {
        l_tft.deleteSprite();
        l_tft.createSprite(this->getWidth(), this->getHeight());
        l_tft.fillSprite(0xFFFF);

        draw();
        
        for (int i = 0; i < children.size(); i++)
        {
            if(children[i] != nullptr && (children[i]->getAbsoluteY() < this->getAbsoluteY() + this->getHeight())  && (this->getAbsoluteY() < children[i]->getAbsoluteY() + children[i]->getHeight())  && (children[i]->getAbsoluteX() < this->getAbsoluteX() + this->getWidth()))
                children[i]->renderAll();
        }
    }

    if(upFromDrawAll==this)
    {
        upFromDrawAll=nullptr;
        l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY());
        drawing=false;
    }
    else
    {
        l_tft.pushSprite(&parent->l_tft, getAbsoluteX(), getAbsoluteY());
    }
}

void Gauss::reloadWidget()
{
    if(parent!=nullptr)
        parent->reloadWidget();
    rendered=false;
}

bool Gauss::updateAll()
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

void Gauss::addChild(Gauss *child)
{
    if(child == nullptr)
        return;
    children.push_back(child);
    child->setParent(this);

    reloadWidget();
}

void Gauss::removechildren()
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

void Gauss::setParent(Gauss *parent)
{
    this->parent = parent;
    reloadWidget();
    rendered=false;
}

Gauss* Gauss::getMaster()
{
    if(parent == nullptr)
        return this;
    else
        return parent->getMaster();
}

int16_t Gauss::getX()
{
    return this->x;
}

int16_t Gauss::getY()
{
    return this->y;
}

int16_t Gauss::getWidth()
{
    return this->width;
}

int16_t Gauss::getHeight()
{
    return this->height;
}

int16_t Gauss::getAbsoluteX()
{
    if(parent==nullptr)
        return getRelativeX();
    else
        return getRelativeX() + parent->getAbsoluteX();
}

int16_t Gauss::getAbsoluteY()
{
    if(parent==nullptr)
        return getRelativeY();
    else
        return getRelativeY() + parent->getAbsoluteY();
}

int16_t Gauss::getRelativeX()
{
    if(parent==nullptr)
        return this->getMarginX() + this->getX();
    else
        return this->getMarginX() + this->getX() + parent->getPaddingX();
}

int16_t Gauss::getRelativeY()
{
    if(parent==nullptr)
        return this->getMarginY() + this->getY();
    else
        return this->getMarginY() + this->getY() + parent->getPaddingY();
}

int16_t Gauss::getMarginX()
{
    return this->marginX;
}

int16_t Gauss::getMarginY()
{
    return this->marginY;
}

int16_t Gauss::getPaddingX()
{
    return this->paddingX;
}

int16_t Gauss::getPaddingY()
{
    return this->paddingY;
}

color_t Gauss::getColor()
{
    return this->color;
}

color_t Gauss::getBackgroundColor()
{
    return this->backgroundColor;
}

color_t Gauss::getBorderColor()
{
    return this->borderColor;
}

int16_t Gauss::getBorderSize()
{
    return this->borderSize;
}

Alignment Gauss::getAlignment()
{
    return this->H_alignment;
}

void Gauss::setX(int16_t x)
{
    this->x=x;
    rendered=false;
}

void Gauss::setY(int16_t y)
{
    this->y=y;
    rendered=false;
}

void Gauss::setWidth(int16_t width)
{
    this->width=width;
    rendered=false;
}

void Gauss::setHeight(int16_t height)
{
    this->height=height;
    rendered=false;
}

void Gauss::setMarginX(int16_t marginX)
{
    this->marginX=marginX;
    rendered=false;
}

void Gauss::setMarginY(int16_t marginY)
{
    this->marginY=marginY;
    rendered=false;
}

void Gauss::setPaddingX(int16_t paddingX)
{
    this->paddingX=paddingX;
    rendered=false;
}

void Gauss::setPaddingY(int16_t paddingY)
{
    this->paddingY=paddingY;
    rendered=false;
}

void Gauss::setColor(color_t color)
{
    this->color=color;
    rendered=false;
}

void Gauss::setBackgroundColor(color_t backgroundColor)
{
    this->backgroundColor=backgroundColor;
    rendered=false;
}

void Gauss::setBorderColor(color_t borderColor)
{
    this->borderColor=borderColor;
    rendered=false;
}

void Gauss::setBorderSize(int16_t borderSize)
{
    this->borderSize=borderSize;
    rendered=false;
}

void Gauss::setTheme(uint8_t theme)
{
    color = theme_color[theme][0];
    backgroundColor = theme_color[theme][1];
    borderColor = theme_color[theme][2];
    rendered=false;
}

void Gauss::setHorizontalAlignment(Alignment alignment)
{
    this->H_alignment=alignment;
    rendered=false;
}

void Gauss::setVerticalAlignment(Alignment alignment)
{
    this->V_alignment=alignment;
    rendered=false;
}

void Gauss::setRadius(int16_t radius)
{
    this->radius=radius;
    rendered=false;
}

int16_t Gauss::getLowestY()
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

int16_t Gauss::getLowestX()
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

int16_t Gauss::getHighestY()
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

int16_t Gauss::getHighestX()
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

Gauss* Gauss::getParent()
{
    return this->parent; // warning, can return nullptr
}

void Gauss::reload()
{
    drawAll();
}

#endif