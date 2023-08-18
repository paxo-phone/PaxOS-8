#include <thread>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>
#include <iostream>

void setup(bool *shouldUpdateScreen);
void loop(void);

bool shouldUpdateScreen = true;

static void loopThread(void)
{
  setup(&shouldUpdateScreen);
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
