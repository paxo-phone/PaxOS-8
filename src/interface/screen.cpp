#include "screen.hpp"

#ifdef ESP32
    LGFX tft_root;
#endif

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    LGFX tft_root(SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

namespace screen
{
    void init(void)
    {
        #ifdef ESP32
            uint16_t calibrationData[] = {316, 194, 307, 3778+300, 3771-200, 204, 3740-200, 3750+300};

            tft_root.setTouchCalibrate(calibrationData);

            pinMode(14, OUTPUT);
            digitalWrite(14, 1);
            pinMode(22, OUTPUT);
            digitalWrite(22, 1);
        #endif
    
        tft_root.init();
    }
}