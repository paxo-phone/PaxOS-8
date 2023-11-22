#include "screen.hpp"

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    LGFX tft_root(WIDTH, HEIGHT);
#endif

#ifdef ESP32
    LGFX tft_root;
#endif
