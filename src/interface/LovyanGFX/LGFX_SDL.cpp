#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include <thread>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>
#include <iostream>
#include <atomic>

struct Rectangle {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};


void setup(bool *shouldUpdateScreen, Rectangle* *screenUpdateZones);

bool shouldUpdateScreen = true;
Rectangle* screenUpdateZones;

void loop(void);

static void loopThread(void)
{
  setup(&shouldUpdateScreen, &screenUpdateZones);
  for (;;)
  {
    std::this_thread::yield();
    loop();
  }
}

//int main(int, char**)
//{
//  std::thread sub_thread(loopThread);
//  for (;;)
//  {
//    std::this_thread::yield();
//    lgfx::Panel_sdl::sdl_event_handler();
//    SDL_Delay(5);
//  }
//}

int main(int, char**)
{
  std::thread sub_thread(loopThread);
  for (;;)
  {
      std::this_thread::yield();
      lgfx::Panel_sdl::sdl_event_handler(&shouldUpdateScreen);
    SDL_Delay(5);
  }
}


#endif