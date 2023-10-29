#ifdef ESP32
    #define BUILD_PAXO 1
#endif

//#define OLD_PAXO
#define NEW_PAXO

#include <iterator>
#ifndef BUILD_PAXO
    #define BUILD_EMU 1
#endif


#ifdef BUILD_PAXO
#include "soc/rtc_wdt.h"
#include "esp_heap_caps.h"
#include <esp_task_wdt.h>
#endif
#ifdef BUILD_EMU
  #include <chrono>

  uint64_t micros() {
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - 1677588060000000;
  }

  uint64_t millis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - 1677588060000;
  }

  void delay(uint64_t ms)
  {
    uint64_t start = millis();
    while (millis() - start < ms);
  }

  int random(int i, int j)
  {
    return (rand()%(j-i))-i;
  }
#endif

#include <stdlib.h>
#include <stdint.h>
#include <iostream>

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

#endif

#include "src/drivers/drivers.hpp"
#include "src/lib/libs.hpp"
#include "src/modules/modules.hpp"
#include "src/apps/app.hpp"

#ifdef BUILD_PAXO

#include "esp_task_wdt.h"

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
    shell::init(); new_thread(CORE_BACK, thread_shell, nullptr);
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

    /*HttpClient client;

    print("get: " + client.get("http://example.com"));*/

    launcher();
}

void loop()
{
}
