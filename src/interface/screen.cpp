#include "screen.hpp"

#ifdef BUILD_EMU
    LGFX tft_root(WIDTH, HEIGHT);
#endif

#ifdef BUILD_PAXO
    LGFX tft_root;
#endif
