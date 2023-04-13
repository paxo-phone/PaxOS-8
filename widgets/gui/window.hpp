#ifndef WINDOW_GUI
#define WINDOW_GUI

#include "box.hpp"

class Window : public Gui
{
    public:
    Window(std::string title)
    {
        this->title = title;
        init(0, 0, 320, 480);
        setBackgroundColor(COLOR_LIGHT);
        setMarginX(10);
        setMarginY(10 + CONTROL_BAR_SIZE);

        bar = new Box(0, 0, 320, CONTROL_BAR_SIZE);
            bar->setBackgroundColor(COLOR_EXTRA_LIGHT);
            bar->setRadius(0);
            bar->setBorderSize(0);
            bar->noMargin = true;
        addChild(bar);
        
        hourLabel = new Label(110, 0, 100, CONTROL_BAR_SIZE, "10:22");
            hourLabel->fontHeight = 20;
            hourLabel->bold = true;
            hourLabel->setTextColor(COLOR_BLACK);
            hourLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
            hourLabel->setVerticalAlignment(CENTER_ALIGNMENT);
            hourLabel->enabledBackground = true;
            hourLabel->setBackgroundColor(COLOR_EXTRA_LIGHT);
            hourLabel->setBorderSize(0);
            hourLabel->setRadius(0);
    }

    void draw()
    {
        for (int i = 0; i < children.size(); i++)
        {
            if (children[i] == bar)
            {
                children.erase(children.begin() + i);
            }
        }
        this->addChild(bar);
        l_tft.fillRect(0, CONTROL_BAR_SIZE+1, 320, 480-CONTROL_BAR_SIZE, getBackgroundColor());
    }

    void afterRender()
    {
        uint16_t maxH = getLowestY();
        uint16_t windowSize = 480 - CONTROL_BAR_SIZE;
        uint16_t slideBarSize = windowSize*windowSize / maxH;
        l_tft.fillRoundRect(320-2-6, CONTROL_BAR_SIZE + 2 + (windowSize)*(-scroolY)/maxH, 6, slideBarSize, 3, COLOR_GREY);
    }

    void virtual_update()
    {
        if (this->getLowestY() > this->getHeight())
        {
            this->verticalSlide = true;
        }
    }

    std::string title = "";
    Box* bar = nullptr;
    Label* hourLabel = nullptr;
};

#endif