#ifndef WINDOW_GUI_CPP
#define WINDOW_GUI_CPP

#include "../gui.hpp"
#include "window.hpp"
#include "box.hpp"
#include "keyboard.hpp"
#include "../../interface/interface.hpp"

#include <string>

Window::Window(std::string title) : Gui(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)
{
    this->title = title;
    setBackgroundColor(COLOR_LIGHT);
    setMarginX(10);
    setMarginY(10 + CONTROL_BAR_SIZE);

    bar = new Box(0, 0, WINDOW_WIDTH, CONTROL_BAR_SIZE);
        bar->setBackgroundColor(COLOR_EXTRA_LIGHT);
        bar->setRadius(0);
        bar->setBorderSize(0);
        bar->noMargin = true;
        bar->enabledBackground=true;
    addChild(bar);
    
    hourLabel = new Label(110, 0, 100, CONTROL_BAR_SIZE, ((gsm.hours>9)?("0"):("")) + to_string(gsm.hours) + ":" + ((gsm.minutes>9)?("0"):("")) + to_string(gsm.minutes));
        hourLabel->setFontSize(20);
        hourLabel->setBold(true);
        hourLabel->setTextColor(COLOR_BLACK);
        hourLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
        hourLabel->setVerticalAlignment(CENTER_ALIGNMENT);
        hourLabel->enabledBackground = true;
        hourLabel->setBackgroundColor(COLOR_EXTRA_LIGHT);
        hourLabel->setBorderSize(0);
        hourLabel->setRadius(0);
        bar->addChild(hourLabel);

    for(int i = 1; i < 5; i++) // load network quality
    {
        netQual[i] = new Image("system/sig_" + to_string(i) + ".png", 4, 4);
        netQual[i]->load();
        netQual[i]->getImage()->canbedeleted = false;
        bar->addChild(netQual[i]);
    }

    for(int i = 0; i < 5; i++) // load battery level
    {
        battLevel[i] = new Image("system/batt_0" + to_string(i+1) + ".png", 279, 6);
        battLevel[i]->load();
        (battLevel[i]->getImage())->canbedeleted = false;
        bar->addChild(battLevel[i]);
    }

    for(int i = 1; i < 5; i++)
    {
        if(gsm.quality == i)
            netQual[i]->enable();
        else
            netQual[i]->disable();
    }

    for(int i = 0; i < 5; i++)
    {
        if(gsm.batteryLevel == i)
            battLevel[i]->enable();
        else
            battLevel[i]->disable();
    }

    keyboard = new Keyboard();
    keyboard->disable();
    addChild(keyboard);

    updateEventId = setInterval(new CallbackMethod<Window>(this, &Window::updateModules), 1000);
}

Window::~Window()
{
    removeInterval(updateEventId);
}

void Window::updateModules()
{
    // hour

    const std::string newHourString = to_string(gsm.hours) + ":" + ((gsm.minutes<=9)?("0"):("")) + to_string(gsm.minutes);
    if(OldnetQual != gsm.quality || OldbattLevel != gsm.batteryLevel || oldhourString != newHourString)
    {
        OldbattLevel = gsm.batteryLevel;
        OldnetQual = gsm.quality;
    
        hourLabel->setText(newHourString);

        for(int i = 1; i < 5; i++) // network quality
        {
            if(gsm.quality == i)
                netQual[i]->enable();
            else
                netQual[i]->disable();
        }

        for(int i = 0; i < 5; i++) // load network quality
        {
            if(gsm.batteryLevel == i)
                battLevel[i]->enable();
            else
                battLevel[i]->disable();
        }

        //print("update modules");
    }
}

void Window::draw()
{
    for (int i = 0; i < children.size(); i++) // bring state bar to front
    {
        if (children[i] == bar)
        {
            children.erase(children.begin() + i);
        }
    }
    this->addChild(bar);
}

void Window::afterRender()
{
    uint16_t maxH = getLowestY();
    uint16_t windowSize = WINDOW_HEIGHT - CONTROL_BAR_SIZE;
    uint16_t slideBarSize = windowSize*windowSize / maxH;
    
    if(maxH > WINDOW_HEIGHT && verticalSlide)
    {
        l_tft.fillRoundRect(WINDOW_WIDTH-2-6, CONTROL_BAR_SIZE + 2 + (windowSize)*(-scroolY)/maxH, 6, slideBarSize, 3, COLOR_GREY);
    }
}

void Window::background_update()
{
    if(mainWindow!=this)
    {
        mainWindow=this;
        renderAll();
    }
    if (this->getLowestY() > this->getHeight())
    {
        this->verticalSlide = true;
    }
    if(widgetPressed != nullptr)
    {
        if(widgetPressed->getType()==LABEL_TYPE && reinterpret_cast<Label*>(widgetPressed)->canBeEdited())
        {
            keyboard->link(reinterpret_cast<Label*>(widgetPressed));
        }else
        {
            if(widgetPressed->getParent()==nullptr || widgetPressed->getParent()->getType()!=KEYBOARD_TYPE)
                keyboard->link(nullptr);
        }
    }
}

#endif