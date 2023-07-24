#ifndef WINDOW_GUI
#define WINDOW_GUI

#include "box.hpp"

class Window : public Gui
{
    public:
    int updateEventId;
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
            bar->enabledBackground=true;
        addChild(bar);
        
        hourLabel = new Label(110, 0, 100, CONTROL_BAR_SIZE, ((gsm.hours>9)?("0"):("")) + to_string(gsm.hours) + ":" + ((gsm.minutes>9)?("0"):("")) + to_string(gsm.minutes));
            hourLabel->fontHeight = 20;
            hourLabel->bold = true;
            hourLabel->setTextColor(COLOR_BLACK);
            hourLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
            hourLabel->setVerticalAlignment(CENTER_ALIGNMENT);
            hourLabel->enabledBackground = true;
            hourLabel->setBackgroundColor(COLOR_EXTRA_LIGHT);
            hourLabel->setBorderSize(0);
            hourLabel->setRadius(0);
            bar->addChild(hourLabel);

        std::string netState[2] = {"", "2G"};
        networkLabel = new Label(0, 0, 100, CONTROL_BAR_SIZE, netState[gsm.network_state]);
            networkLabel->fontHeight = 20;
            networkLabel->bold = true;
            networkLabel->setTextColor(COLOR_BLACK);
            networkLabel->setVerticalAlignment(CENTER_ALIGNMENT);
            networkLabel->enabledBackground = true;
            networkLabel->setBackgroundColor(COLOR_EXTRA_LIGHT);
            networkLabel->setBorderSize(0);
            networkLabel->setRadius(0);
            bar->addChild(networkLabel);

        updateEventId = setInterval(new CallbackMethod<Window>(this, &Window::updateModules), 1000);
    }

    ~Window()
    {
        removeInterval(updateEventId);
    }

    void updateModules()
    {
        // hour
        hourLabel->setText(to_string(gsm.hours) + ":" + ((gsm.minutes<=9)?("0"):("")) + to_string(gsm.minutes));
        std::string netState[2] = {"", "2G"};
        networkLabel->setText(netState[gsm.network_state]);
    }

    void draw()
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

    void afterRender()
    {
        uint16_t maxH = getLowestY();
        uint16_t windowSize = 480 - CONTROL_BAR_SIZE;
        uint16_t slideBarSize = windowSize*windowSize / maxH;
        
        if(maxH > 480)
        {
            l_tft.fillRoundRect(320-2-6, CONTROL_BAR_SIZE + 2 + (windowSize)*(-scroolY)/maxH, 6, slideBarSize, 3, COLOR_GREY);
        }
    }

    void virtual_update()
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
    }

    std::string title = "";
    Box* bar = nullptr;
    Label* hourLabel = nullptr;
    Label* networkLabel = nullptr;
};

#endif