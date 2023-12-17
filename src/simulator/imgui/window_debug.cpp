//
// Created by Charlito33 on 16/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "window_debug.hpp"

#include "imgui.h"

#include "window_simulator.hpp"

int simulator::imgui::window::debug::batteryLevel = 4;
int simulator::imgui::window::debug::networkQuality = 4;

bool simulator::imgui::window::debug::automaticDate = true;
int simulator::imgui::window::debug::dateYears = 2023; // 2023
int simulator::imgui::window::debug::dateMonths = 12; // December
int simulator::imgui::window::debug::dateDays = 17; // 17th
int simulator::imgui::window::debug::dateHours = 13; // 13h
int simulator::imgui::window::debug::dateMinutes = 51; // 51m
int simulator::imgui::window::debug::dateSeconds = 33; // 33s

void simulator::imgui::window::debug::render() {
    if (!ImGui::Begin("Debug")) {
        ImGui::End();
        return;
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

    if (ImGui::CollapsingHeader("Components")) {
        if (ImGui::TreeNode("Sim800L")) {
            ImGui::SliderInt("Battery Level", &batteryLevel, 0, 4);
            ImGui::SliderInt("Network Quality", &networkQuality, 1, 4);

            if (ImGui::TreeNode("Date")) {
                ImGui::Checkbox("Auto", &automaticDate);

                ImGui::SliderInt("Years", &dateYears, 1900, 2100);
                ImGui::SliderInt("Months", &dateMonths, 1, 12);
                ImGui::SliderInt("Days", &dateDays, 1, 31);
                ImGui::SliderInt("Hours", &dateHours, 0, 23);
                ImGui::SliderInt("Minutes", &dateMinutes, 0, 59);
                ImGui::SliderInt("Seconds", &dateSeconds, 0, 60);

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }

    ImGui::End();
}