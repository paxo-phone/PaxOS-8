#ifndef THREAD_HPP
#define THREAD_HPP

#define CORE_APP 0
#define CORE_BACK 1

#ifdef BUILD_EMU

#include <thread>

void new_thread(bool core, void(*func)(void*), void* arg)
{
    std::thread myThread(func, arg);
}

#endif

#ifdef BUILD_PAXO

void new_thread(bool core, void(*func)(void*), void* arg)
{
    xTaskCreate(func,
              "new_thread",
              50000,
              NULL,
              0,
              NULL);
}

#endif

#endif