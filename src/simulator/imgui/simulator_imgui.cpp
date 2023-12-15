//
// Created by Charlito33 on 15/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "simulator_imgui.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

SDL_Window *simulator::imgui::sdlWindow;
SDL_Renderer *simulator::imgui::sdlRenderer;

void simulator::imgui::init(SDL_Window *w, SDL_Renderer *r) {
    sdlWindow = w;
    sdlRenderer = r;

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(sdlWindow, sdlRenderer);
    ImGui_ImplSDLRenderer2_Init(sdlRenderer);
}

void simulator::imgui::shutdown() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void simulator::imgui::forwardEvent(SDL_Event *event) {
    ImGui_ImplSDL2_ProcessEvent(event);
}

void simulator::imgui::beginDraw() {
    // Begin ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();



    ImGui::ShowDemoWindow();
}

void simulator::imgui::endDraw() {
    // Draw ImGui frame
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}