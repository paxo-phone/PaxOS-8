//
// Created by Charlito33 on 16/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "window_debug.hpp"

#include "imgui.h"

#include "window_simulator.hpp"

void simulator::imgui::window::debug::render() {
    if (!ImGui::Begin("Debug")) {
        ImGui::End();
    }

    if (ImGui::CollapsingHeader("Position")) {
        if (ImGui::TreeNode("Mouse")) {
            ImVec2 mousePosRaw = ImGui::GetMousePos();

            int mouseX = static_cast<int>(mousePosRaw.x);
            int mouseY = static_cast<int>(mousePosRaw.y);

            window::simulator::offsetClick(&mouseX, &mouseY);

            auto mousePosScreen = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));

            ImGui::Text("Mouse Pos (raw) : %.f, %.f", mousePosRaw.x, mousePosRaw.y);
            ImGui::Text("Mouse Pos (screen) : %.f, %.f", mousePosScreen.x, mousePosScreen.y);

            ImGui::TreePop();
        }

    }

    ImGui::End();
}