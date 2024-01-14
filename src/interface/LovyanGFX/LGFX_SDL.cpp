#include <stdint.h>
#include <string.h>

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include <thread>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>
#include <iostream>

void setup(bool *shouldUS);

void loop(void);

static void loopThread(void)
{
  // Will be available globally with pShouldUpdateScreen defined in main.cpp.
  bool SDLShouldUpdateScreen = true;
  setup(&SDLShouldUpdateScreen);
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
  while (true)
  {
    std::this_thread::yield();
    bool s = true;
    lgfx::Panel_sdl::sdl_event_handler(&s);
    SDL_Delay(5);
  }
}


#endif
