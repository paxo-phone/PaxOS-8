#include "includes.h"

#include <iterator>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iostream>

using namespace std;

#ifdef BUILD_EMU
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

#ifdef BUILD_PAXO
void setup() // initialize paxos v8
{
    //esp_task_wdt_init(10000, 0);
    esp_task_wdt_init(30, true);

    // Disable the task watchdog timer
    esp_task_wdt_deinit();
#endif
    
#ifdef BUILD_EMU
void setup(bool *shouldUpdateScreen, Rectangle* *screenUpdateZones) // initialize paxos v8
    {
    shouldUS = shouldUpdateScreen;
    screenUZ = screenUpdateZones;
#endif


    Gui::initScreen();
    shell::init();
        new_thread(CORE_BACK, thread_shell, nullptr);
    storage::init();
    screen_light.init();
    gsm.init();
        gsm.saveMessages = Message::saveMessage;
    home_button.init();

    /*LuaInterpreter lua;
    lua.loadScript("apps/lua/test/main.lua");
    lua.runApp();*/

    /*webdriver.powerOn();
    webdriver.connect();

    while(!webdriver.isConnected())
    {
        print ("waiting connection");
    }*/

    // HttpClient client;
    // print("get: " + client.get("http://example.com"));

    storage::FileStream output;
    output.open("system/file.txt", storage::WRITE);

    output << "This is my first line" << "\n";
    output << "This is my second line";
    output.close();

    storage::FileStream input;
    input.open("system/file.txt", storage::READ);

    std::string word_1;
    std::string word_2;

    input >> word_1 >> word_2;
    std::cout << "word_1 -> " << word_1 << std::endl;
    std::cout << "word_2 -> " << word_2 << std::endl;

    input.close();

    /*
        output:
        word_1 -> This
        word_2 -> is
    */


    launcher();
}

void loop()
{
}
