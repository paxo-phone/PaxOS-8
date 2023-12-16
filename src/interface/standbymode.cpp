#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include "standbymode.hpp"

#define NORMAL_CPU_FREQUENCY 240
#define FREEZE_CPU_FREQUENCY 40

namespace standbymode
{
    void enable(void)
    {
        #ifdef ESP32
            setCpuFrequencyMhz(FREEZE_CPU_FREQUENCY);
            Serial.end();
            Serial.begin(115200);
            gsm.init();
        #endif
    }

    void disable(void)
    {
        #ifdef ESP32
            setCpuFrequencyMhz(NORMAL_CPU_FREQUENCY);
            Serial.end();
            Serial.begin(115200);
            gsm.init();
        #endif
    }
}