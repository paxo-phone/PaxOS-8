//
// Created by Charlito33 on 12/08/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "Clock.hpp"

std::string getTime() {
    return std::to_string(gsm.hours) + ":" + std::to_string(gsm.minutes) + ":" + std::to_string(gsm.seconds);
}

void Clock::main() {
    Window window("clock");
    window.setMarginX(0);
    window.setMarginY(CONTROL_BAR_SIZE);

    Label hourLabel(0, 20, 308, 40, getTime());
    hourLabel.setHorizontalAlignment(CENTER_ALIGNMENT);
    hourLabel.fontHeight = 32;

    Box drawer1(-4, 404, 80, 40);
    drawer1.enabledBackground = true;
    drawer1.setBackgroundColor(COLOR_EXTRA_LIGHT);
    drawer1.setBorderColor(COLOR_BLACK);
    drawer1.setBorderSize(2);

    window.addChild(&hourLabel);
    //window.addChild(&drawer1);

    while (!home_button.pressed()) {
        // Update elements
        hourLabel.setText(getTime());

        window.updateAll();

#ifdef BUILD_EMU
        SDL_Delay(20);
#endif
    }
}
