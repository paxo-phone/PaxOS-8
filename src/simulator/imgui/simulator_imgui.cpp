//
// Created by Charlito33 on 15/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "simulator_imgui.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "window_simulator.hpp"
#include "window_debug.hpp"
#include "window_console.hpp"

SDL_Window *simulator::imgui::sdlWindow;
SDL_Renderer *simulator::imgui::sdlRenderer;
SDL_Texture *simulator::imgui::screenTexture;
ImVec2 simulator::imgui::screenSize;
bool simulator::imgui::aboutWindow;

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

void simulator::imgui::setScreenTexture(SDL_Texture *texture, int width, int height) {
    screenTexture = texture;
    screenSize = ImVec2(static_cast<float>(width), static_cast<float>(height));
}

void simulator::imgui::beginDraw() {
    // Begin ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        ImGui::MenuItem("About", nullptr, &aboutWindow);

        ImGui::EndMainMenuBar();
    }

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::ShowDemoWindow();

    window::simulator::render(screenTexture, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
    window::debug::render();
    window::console::render();

    if (aboutWindow) {
        ImGui::Begin("About", &aboutWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);

        ImGui::Text("PaxOS 8.0 by Paxo");
        ImGui::Separator();
        ImGui::Text("Website : https://paxo.fr/");
        ImGui::Text("Contribute : https://github.com/paxo-phone/PaxOS-8");
        ImGui::Separator();
        ImGui::Text("Libraries :");
        ImGui::Text("- LovyanGFX by lovyan03 - Custom version");
        ImGui::Text("- SDL2 by libsdl.org - v2.28.2");
        ImGui::Text("- ImGui by Omar Cornut - v1.90.1 WIP 19002");
        ImGui::Text("- Lua by Lua.org / PUC-Rio - v5.4.6");
        ImGui::Text("- CURL by Curl Corporation - v8.5.0-1");
        ImGui::Text("- Dirent by Toni Rönkkö - v1.23.2");
        ImGui::Text("");
        ImGui::Text("Missing credits ? Contact us at paxo.fr");

        ImGui::End();
    }
}

void simulator::imgui::endDraw() {
    // Draw ImGui frame
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}