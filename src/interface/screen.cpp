//
//  screen.cpp
//  PaxOS8
//
//  Created by Antoine Bollengier on 12.11.2023.
//

#include "screen.hpp"

#ifdef BUILD_EMU
    LGFX tft_root(WIDTH, HEIGHT);
#endif

#ifdef BUILD_PAXO
    LGFX tft_root;
#endif
