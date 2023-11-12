#ifndef SCREEN_HEADER
#define SCREEN_HEADER

#define WIDTH 320
#define HEIGHT 480

#ifdef BUILD_EMU
    #define LGFX_USE_V1
    #include "LovyanGFX/LovyanGFX.hpp"
    #include "LovyanGFX/LGFX_AUTODETECT.hpp"
    //#include "../sdl_main.cpp"
    extern LGFX tft_root;
#endif
#ifdef BUILD_PAXO
    #define LGFX_USE_V1
    #include <LovyanGFX.hpp>
    #include "LovyanGFX/lovyangfx_config.hpp"
    extern LGFX tft_root;
#endif

#endif
