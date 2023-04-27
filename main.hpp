#define BUILD_PAXO 1
#include <iterator>
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
    gsm.init();
    home_button.init();

    gsm.askForHour();

    launcher();
}

void loop()
{
    #ifdef BUILD_PAXO
    vTaskDelay(1);
    #endif
}