#ifndef THREAD_HPP
#define THREAD_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#define CORE_APP 0
#define CORE_BACK 1

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include <thread>

#endif

void new_thread(bool core, void(*func)(void*), void* arg);

#endif
