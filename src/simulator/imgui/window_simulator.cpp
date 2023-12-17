//
// Created by Charlito33 on 15/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "window_simulator.hpp"

ImVec2 simulator::imgui::window::simulator::cursorPos;
bool simulator::imgui::window::simulator::homeButtonPressed = false;
bool simulator::imgui::window::simulator::aboutWindow = false;
float simulator::imgui::window::simulator::screenScale = 1;

void simulator::imgui::window::simulator::render(SDL_Texture *texture, int width, int height) {
    if (!ImGui::Begin("Simulator", nullptr, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
    }

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 screenSize = ImVec2(static_cast<float>(width) * screenScale, static_cast<float>(height) * screenScale);

    ImGui::SetCursorPosX((windowSize.x - screenSize.x) * 0.5f); // Center horizontally
    cursorPos = ImGui::GetCursorScreenPos(); // Get pos before drawing the image, so the image x & y on screen
    ImGui::ImageButton(texture, screenSize); // Use ImageButton to disable ImGui input handler

    ImGui::SetCursorPosX((windowSize.x - screenSize.x) * 0.5f);
    homeButtonPressed = ImGui::Button("Back", ImVec2(screenSize.x + 8, 50));

    ImGui::Separator();

    ImGui::Text("Config");
    ImGui::SliderFloat("Scale", &screenScale, 0.1, 5, "%.2f");

    ImGui::End();
}

void simulator::imgui::window::simulator::offsetClick(int *x, int *y) {
    *x = static_cast<int>(static_cast<float>(*x - static_cast<int>(cursorPos.x)) / screenScale);
    *y = static_cast<int>(static_cast<float>(*y - static_cast<int>(cursorPos.y)) / screenScale);
}