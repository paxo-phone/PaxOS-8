#define BUILD_PAXO 1
#ifndef BUILD_PAXO
    #define BUILD_EMU 1
#endif

#ifdef BUILD_PAXO
#include "soc/rtc_wdt.h"
#include "esp_heap_caps.h"
#include <esp_task_wdt.h>
#endif

#include <stdlib.h>
#include <stdint.h>

using namespace std;

#include "interface/interface.hpp"
#include "widgets/gui.hpp"
#include "tasks/tasks.hpp"
#include "extensions/extensions.hpp"
#include "app/app.hpp"

Window* box = nullptr;
Label* label = nullptr;
Label* label2 = nullptr;
Label* label3 = nullptr;

void setup() // initialize the paxos v8
{
    #ifdef BUILD_PAXO
    esp_task_wdt_init(10000, 0);
    #endif

    Gui::initScreen();
    shell::init(); new_thread(CORE_BACK, thread_shell, nullptr);
    storage::init();

    box = new Window("test");
    
    label = new Button(0, 0, AUTO, 50, "Button");
    box->addChild(label);

    Image* image = new Image("launcher.png", AUTO, AUTO, 32*4, 48*2);
    image->load();
    box->addChild(image);

    for (int i = 0; i < 5; i++)
    {
        Label* label2 = new Label(AUTO, AUTO, AUTO, 100, "this text is an example of text on the paxos 8");
        label2->setTextColor(COLOR_BLACK);
        box->addChild(label2);
    }

    //box->verticalSlide = true;
}

void loop()
{
    #ifdef BUILD_PAXO
    vTaskDelay(1);
    #endif

    box->updateAll();
    eventHandler.update();

    if (label->isTouched())
    {
        print("Press");
    }
}