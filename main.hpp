//#define BUILD_PAXO 1
#define BUILD_EMU 1

#include <stdlib.h>
#include <stdint.h>

using namespace std;

#include "interface/interface.hpp"
#include "widgets/gui.hpp"
#include "tasks/tasks.hpp"

Box* box = nullptr;
Label* label = nullptr;
Label* label2 = nullptr;
Label* label3 = nullptr;

bool state = false;

void setup()
{
    Gui::initScreen();
    storage::init();

    box = new Box(10, 10, 300, 460);
    
    label = new Label(0, AUTO, AUTO, 200, "A");
    label->fontHeight = 1;
    box->addChild(label);
    
    box->setBackgroundColor(COLOR_ERROR);
    box->renderAll();

    while(!box->isTouched())
    {
        box->updateAll();
    }
}

void loop()
{
    box->updateAll();

    delay(100);
    label->fontHeight++;
    box->renderAll();
}