//
//  thread.cpp
//  PaxOS8
//
//  Created by Antoine Bollengier on 12.11.2023.
//

#include "thread.hpp"

#ifdef BUILD_EMU

void new_thread(bool core, void(*func)(void*), void* arg)
{
    std::thread myThread(func, arg);
    myThread.detach();
}

#endif

#ifdef BUILD_PAXO

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
