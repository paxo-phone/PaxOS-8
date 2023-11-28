#include <iterator>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iostream>

using namespace std;

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    #include <atomic>

    struct Rectangle {
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
    };

    bool *shouldUS;
    Rectangle* *screenUZ;

    void flushScreen()
    {
        *shouldUS = true;
    }
#endif

/*
    HEADERS OF THE PAXOS_8
*/

#include "interface/interface.hpp"
#include "widgets/gui.hpp"
#include "tasks/tasks.hpp"
#include "lua/lua.hpp"
#include "app/app.hpp"
#include "web/web.hpp"
#include "app/message/message.hpp"

// cette partie est simplement pour activer les processus qui different entre arduino / pc

#ifdef ESP32
void setup() // initialize paxos v8
{
    //esp_task_wdt_init(10000, 0);
    esp_task_wdt_init(30, true);

    // Disable the task watchdog timer
    esp_task_wdt_deinit();
#endif
    
#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
void setup(bool *shouldUpdateScreen, Rectangle* *screenUpdateZones) // initialize paxos v8
    {
    shouldUS = shouldUpdateScreen;
    screenUZ = screenUpdateZones;
#endif


    Gui::initScreen();
    // new_thread(CORE_BACK, shell::thread_shell, nullptr);
    storage::init();
    light::init();
    gsm.init();
        gsm.saveMessages = Message::saveMessage;
    home_button::init();


    launcher();
}

void loop()
{
}
