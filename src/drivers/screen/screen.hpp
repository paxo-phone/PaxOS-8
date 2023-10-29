#ifndef SCREEN_HEADER
#define SCREEN_HEADER

#ifdef BUILD_EMU
    #include "LGFX_SDL.cpp"
#endif
#ifdef BUILD_PAXO
    #define LGFX_USE_V1
    #include <LovyanGFX.hpp>
    #include "LovyanGFX/lovyangfx_config.hpp"
#endif


#define WIDTH 320
#define HEIGHT 480

#ifdef BUILD_EMU
    LGFX tft_root(WIDTH, HEIGHT);
#endif
#ifdef BUILD_PAXO
    LGFX tft_root;
#endif

#endif