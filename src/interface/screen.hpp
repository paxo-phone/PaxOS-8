#ifndef SCREEN_HPP
#define SCREEN_HPP

#define LGFX_USE_V1

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif 

#ifdef ESP32 
    #include "LovyanGFX/LovyanGFX.hpp"
    #include "LovyanGFX/lovyangfx_config.hpp"
#endif

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    #include "LovyanGFX/LovyanGFX.hpp"
    #include "LovyanGFX/LGFX_AUTODETECT.hpp"
#endif

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

extern LGFX tft_root;

#endif /* SCREEN_HPP */
