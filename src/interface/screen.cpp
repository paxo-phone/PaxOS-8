#include "screen.hpp"

#ifdef ESP32
    LGFX tft_root;
#endif

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    LGFX tft_root(SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

