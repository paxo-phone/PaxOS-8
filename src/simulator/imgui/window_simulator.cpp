//
// Created by Charlito33 on 15/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "window_simulator.hpp"

#include "../../interface/console.hpp"

ImVec2 simulator::imgui::window::simulator::cursorPos;
bool simulator::imgui::window::simulator::homeButtonPressed = false;
bool simulator::imgui::window::simulator::aboutWindow = false;
float simulator::imgui::window::simulator::screenScale = 1;
bool simulator::imgui::window::simulator::autoScale = true;

void simulator::imgui::window::simulator::render(SDL_Texture *texture, int width, int height) {
    if (!ImGui::Begin("Simulator", nullptr, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Config")) {
            if (ImGui::BeginMenu("Scale")) {
                ImGui::Checkbox("Auto-Scale", &autoScale);
                ImGui::SliderFloat("Scale", &screenScale, 0.1, 5, "%.2f");

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    const ImVec2 windowSize = ImGui::GetWindowSize();
    auto screenSize = ImVec2(static_cast<float>(width) * screenScale, static_cast<float>(height) * screenScale);

    if (autoScale) {
        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        availableSize.y -= 50 + 10; // 10 is for margin between components

        const float ratio = static_cast<float>(width) / static_cast<float>(height);
        const float availableSizeRatio = availableSize.x / availableSize.y;

        if (availableSizeRatio > ratio) {
            // Limited by height
            screenSize.x = static_cast<float>(width) * (availableSize.y / static_cast<float>(height));
            screenSize.y = availableSize.y;
        } else if (availableSizeRatio < ratio) {
            // Limited by width
            screenSize.x = availableSize.x;
            screenSize.y = static_cast<float>(height) * (availableSize.x / static_cast<float>(width));
        } else {
            // Not limited
            screenSize.x = availableSize.x;
            screenSize.y = availableSize.y;
        }

        // Update scale in ImGui
        screenScale = screenSize.x / static_cast<float>(width);
    }

    ImGui::SetCursorPosY((windowSize.y - screenSize.y - 25) * 0.5f); // Center vertically
    ImGui::SetCursorPosX((windowSize.x - screenSize.x) * 0.5f); // Center horizontally
    cursorPos = ImGui::GetCursorScreenPos(); // Get pos before drawing the image, so the image x & y on screen
    ImGui::ImageButton(texture, screenSize); // Use ImageButton to disable ImGui input handler

    ImGui::SetCursorPosX((windowSize.x - screenSize.x) * 0.5f); // Center horizontally, ImGui automatically place it under the image
    homeButtonPressed = ImGui::Button("Back", ImVec2(screenSize.x + 8, 50));

    ImGui::End();
}

void simulator::imgui::window::simulator::offsetClick(int *x, int *y) {
    *x = static_cast<int>(static_cast<float>(*x - static_cast<int>(cursorPos.x)) / screenScale);
    *y = static_cast<int>(static_cast<float>(*y - static_cast<int>(cursorPos.y)) / screenScale);
}