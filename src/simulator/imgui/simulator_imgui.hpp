//
// Created by Charlito33 on 15/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#ifndef PAXOS_SIMULATOR_IMGUI_HPP
#define PAXOS_SIMULATOR_IMGUI_HPP

#include "SDL.h"
#include "imgui.h"

namespace simulator::imgui {
    extern SDL_Window *sdlWindow;
    extern SDL_Renderer *sdlRenderer;
    extern SDL_Texture *screenTexture;
    extern ImVec2 screenSize;
    extern bool aboutWindow;

    void init(SDL_Window *window, SDL_Renderer *renderer);
    void shutdown();

    void forwardEvent(SDL_Event *event);
    void setScreenTexture(SDL_Texture *texture, int width, int height);

    void beginDraw();
    void endDraw();
}

#endif //PAXOS_SIMULATOR_IMGUI_HPP
