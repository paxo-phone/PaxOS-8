//
// Created by Charlito33 on 15/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#ifndef PAXOS_WINDOW_SIMULATOR_HPP
#define PAXOS_WINDOW_SIMULATOR_HPP

#include "SDL.h"
#include "imgui.h"

namespace simulator::imgui::window::simulator {
    extern ImVec2 cursorPos;
    extern bool homeButtonPressed;
    extern bool aboutWindow;
    extern float screenScale;

    void render(SDL_Texture *texture, int width, int height);
    void offsetClick(int *x, int *y);
}

#endif //PAXOS_WINDOW_SIMULATOR_HPP
