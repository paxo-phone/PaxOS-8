#include "thread.hpp"

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

void new_thread(bool core, void(*func)(void*), void* arg)
{
    std::thread myThread(func, arg);
    myThread.detach();
}

#endif

#ifdef ESP32

void new_thread(bool core, void(*func)(void*), void* arg)
{
    xTaskCreate(func,
              "new_thread",
              50000,
              nullptr,
              0,
              nullptr);
}

#endif
