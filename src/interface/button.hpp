#ifndef BUTTON_HARD_HPP
#define BUTTON_HARD_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#include "interface.hpp"


class HomeButton
{
    public:
    void init();
    void update();
    void clear();
    bool pressed();

    uint16_t state = 0;

    private:
    unsigned int button_chrono = 0;

    public:
    // standby mod

    uint64_t timer = 0;
    uint32_t timer_delay = 30000; // milliseconds

    void resetStandbyMod();
    bool needStandbyMod();
};

extern HomeButton home_button;

#endif
