//
// Created by Charlito33 on 15/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "window_simulator.hpp"

ImVec2 simulator::imgui::window::simulator::cursorPos;
bool simulator::imgui::window::simulator::homeButtonPressed;
bool simulator::imgui::window::simulator::aboutWindow;

void simulator::imgui::window::simulator::render(SDL_Texture *texture, int width, int height) {
    if (!ImGui::Begin("Simulator", nullptr, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Other")) {
            ImGui::MenuItem("About", nullptr, &aboutWindow);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    cursorPos = ImGui::GetCursorScreenPos(); // Get pos before drawing the image, so the image x & y on screen

    // Use ImageButton to disable ImGui input handler
    ImGui::ImageButton(texture, ImVec2(static_cast<float>(width), static_cast<float>(height)));

    homeButtonPressed = ImGui::Button("Back", ImVec2(static_cast<float>(width) + 8, 50));

    if (ImGui::CollapsingHeader("Debug")) {
        if (ImGui::TreeNode("Position")) {
            ImVec2 mousePosRaw = ImGui::GetMousePos();

            int mouseX = static_cast<int>(mousePosRaw.x);
            int mouseY = static_cast<int>(mousePosRaw.y);

            offsetClick(&mouseX, &mouseY);

            auto mousePosScreen = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));

            ImGui::Text("Mouse Pos (raw) : %.f, %.f", mousePosRaw.x, mousePosRaw.y);
            ImGui::Text("Mouse Pos (screen) : %.f, %.f", mousePosScreen.x, mousePosScreen.y);

            ImGui::TreePop();
        }

    }

    ImGui::End();

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

void simulator::imgui::window::simulator::offsetClick(int *x, int *y) {
    *x = *x - static_cast<int>(cursorPos.x);
    *y = *y - static_cast<int>(cursorPos.y);
}