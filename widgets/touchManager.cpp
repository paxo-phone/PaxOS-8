#ifndef TOUCH_MANAGER_CPP
#define TOUCH_MANAGER_CPP

#include "touchManager.hpp"
#include "gui.hpp"

void TouchManager::update()
{
    if(timerUpdate+10<millis())
    {
        uint16_t z = tft_root.getTouchRaw(&tx, &ty);
        //print ("touch: " + to_string(tx) + "; " + to_string(ty));
        tft_root.convertRawXY(&tx, &ty);
        tx = tx*320/340;
        ty = ty*480/460;
        //tft_root.drawCircle(tx, ty, 3, 0x0000);
        
        if(tx<0 || tx>320 || ty<0 || ty>480)
        {
            tx = 0; ty = 0;
        }
        if (z && lastClick + timeToWait > millis())
        {
            lastClick = millis();
            home_button.resetStandbyMod();
            stateTouch = 1;
        }
        if (z)
        {
            lastClick = millis();
            home_button.resetStandbyMod();
            stateTouch = 1;
        }
        if (!z && lastClick + timeToWait < millis())
        {
            tx = 0;
            ty = 0;
            stateTouch = 0;
        }
        else
        {
            stateTouch = 1;
        }
        if(tx<0 || tx>320 || ty<0 || ty>480)
        {
            tx = 0; ty = 0;
        }
        #ifdef BUILD_PAXO
            //ty = 480 - ty;
        #endif
        timerUpdate=millis();
    }
}

void TouchManager::calibrate()
{
    
}

uint16_t TouchManager::getX()
{
    return tx;
}

uint16_t TouchManager::getY()
{
    return ty;
}

bool TouchManager::isTouch()
{    
    if(stateTouch)
    {
        if(!isPreviousTouch)
        {
            fx=tx;
            fy=ty;
            isPreviousTouch=true;
        }
        if(!stateSlider && (isSlidingHorizontally(true)!=0 || isSlidingVertically(true)!=0))
        {
            stateSlider=1;
        }
    }
    else
    {
        stateSlider=0;
        tx=0;
        ty=0;
        fx=tx;
        fy=ty;
        isPreviousTouch=false;
    }
    
    return stateTouch;
}

int16_t TouchManager::isSlidingHorizontally(bool force)
{
    if((force ? true : isTouch()) && (stateSlider==1 || force) && (fx- ((force) ? (FLUIDITY_START):(FLUIDITY)) >tx || fx+((force) ? (FLUIDITY_START):(FLUIDITY))<tx))
    {
        int16_t pixels = fx-tx;
        return pixels;
    }
        
    return false;
}

int16_t TouchManager::isSlidingVertically(bool force)
{
    if((force ? true : isTouch()) && (stateSlider==1 || force) && (fy+((force) ? (FLUIDITY_START):(FLUIDITY))<ty || fy-((force) ? (FLUIDITY_START):(FLUIDITY))>ty))
    {
        int16_t pixels = fy-ty;
        return pixels;
    }

    return false;
}

void TouchManager::resetScrollVertical()
{
    fy=ty;
}

void TouchManager::resetScrollHorizontal()
{
    fx=tx;
}

bool TouchManager::isTouchRead() // simple detector
{
    update();
    uint16_t z = tft_root.getTouchRaw(&tx, &ty);
    //print(std::to_string(z));
    tft_root.convertRawXY(&tx, &ty);

    if (z)
    {
        home_button.resetStandbyMod();
    }
    
    if(tx<0 || tx>320 || ty<0 || ty>480)
    {
        tx = 0; ty = 0;
    }
    if (z && lastClick + timeToWait > millis())
    {
        lastClick = millis();
        return 1;
    }
    if (z)
    {
        lastClick = millis();
        return 1;
    }
    if (!z && lastClick + timeToWait < millis())
    {
        tx = 0;
        ty = 0;
        return 0;
    }
    else
    {
        return 1;
    }
}

bool TouchManager::validTouch()
{
    bool state = isTouchRead();

    return state;
}

bool Touched(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t xx = touch.getX();
    uint16_t yy = touch.getY();
    return x<xx && xx<x+width && y<yy && yy<y+height;
}

#endif // TOUCH_MANAGER