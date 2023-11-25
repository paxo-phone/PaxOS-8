#ifndef SCREEN_HEADER
#define SCREEN_HEADER

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#define WIDTH 320
#define HEIGHT 480

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    #define LGFX_USE_V1
    #include "LovyanGFX/LovyanGFX.hpp"
    #include "LovyanGFX/LGFX_AUTODETECT.hpp"
    //#include "../sdl_main.cpp"
    extern LGFX tft_root;
#endif
#ifdef ESP32
    #define LGFX_USE_V1
    //#include <LovyanGFX.hpp>
    #include "LovyanGFX/LovyanGFX.hpp"
    #include "LovyanGFX/lovyangfx_config.hpp"
    extern LGFX tft_root;
#endif

#endif
