/*
    THIS IS THE FILE TO CONFIGURATE THE PAXOS
*/

/*
    DEFINE HERE THE MODEL OF THE PAXO (OLD=V4, NEW=V4+)
*/

//#define OLD_PAXO
#define NEW_PAXO


/*
    AUTOMATIC CONFIGURATION FOR ESP32 / EMULATOR
*/

#ifdef ESP32
    #define BUILD_PAXO 1
#else
    #define BUILD_EMU 1
#endif

#ifdef BUILD_PAXO
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#ifdef NEW_PAXO
    #define HOME_BUTTON 33
#endif
#ifdef OLD_PAXO
    #define HOME_BUTTON 32
#endif

#include <stdint.h>
#include <string.h>