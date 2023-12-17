//
// Created by Charlito33 on 17/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "window_console.hpp"

#include "imgui.h"

#include <ctime>

ImU32 simulator::imgui::window::console::infoColor = IM_COL32(255, 255, 255, 255);
ImU32 simulator::imgui::window::console::warnColor = IM_COL32(255, 255, 0, 255);
ImU32 simulator::imgui::window::console::errorColor = IM_COL32(255, 0, 0, 255);
ImU32 simulator::imgui::window::console::debugColor = IM_COL32(0, 255, 0, 255);

bool simulator::imgui::window::console::autoScroll = true;
std::vector<simulator::imgui::window::console::LogEntry *> simulator::imgui::window::console::lines = std::vector<LogEntry *>();

ImU32 getColor(const simulator::imgui::window::console::LogType type) {
    switch (type) {
        case simulator::imgui::window::console::t_info:
            return simulator::imgui::window::console::infoColor;
        case simulator::imgui::window::console::t_warn:
            return simulator::imgui::window::console::warnColor;
        case simulator::imgui::window::console::t_error:
            return simulator::imgui::window::console::errorColor;
        case simulator::imgui::window::console::t_debug:
            return simulator::imgui::window::console::debugColor;
    }

    return 0;
}

std::string getLogType(const simulator::imgui::window::console::LogType type) {
    switch (type) {
        case simulator::imgui::window::console::t_info:
            return "INFO";
        case simulator::imgui::window::console::t_warn:
            return "WARNING";
        case simulator::imgui::window::console::t_error:
            return "ERROR";
        case simulator::imgui::window::console::t_debug:
            return "DEBUG";
    }

    return "";
}

std::string intFormat(const int value) {
    if (value < 10) {
        return "0" + std::to_string(value);
    }

    return std::to_string(value);
}

std::string getLogDate() {
    const std::time_t t = std::time(nullptr);
    const std::tm *now = std::localtime(&t);

    return intFormat(now->tm_hour) + ":" + intFormat(now->tm_min) + ":" + intFormat(now->tm_sec);
}

void simulator::imgui::window::console::render() {
    if (!ImGui::Begin("Console")) {
        ImGui::End();
        return;
    }

    ImGui::Checkbox("Auto scroll", &autoScroll);

    // Create a child, to contain every line of the console output
    // Display the scrollbar only if the auto scroll is off
    ImGui::BeginChild(ImGui::GetID("Console"), ImGui::GetContentRegionAvail(), ImGuiChildFlags_None, autoScroll ? ImGuiWindowFlags_NoScrollbar : ImGuiWindowFlags_None);

    for (const auto& entry : lines) {
        ImGui::PushStyleColor(ImGuiCol_Text, getColor(entry->type));
        ImGui::Text(entry->value.c_str());
        ImGui::PopStyleColor();
    }

    if (autoScroll) {
        ImGui::SetScrollY(ImGui::GetScrollMaxY());
    }

    ImGui::EndChild();

    ImGui::End();
}

void simulator::imgui::window::console::appendRaw(const LogType type, const std::string& value) {
    auto *entry = new LogEntry;
    entry->type = type;
    entry->value = "[" + getLogDate() + "] [" + getLogType(type) + "] " + value;

    lines.insert(lines.end(), entry);
}

void simulator::imgui::window::console::info(const std::string& str) {
    appendRaw(t_info, str);
}

void simulator::imgui::window::console::info(const char* str) {
    info(std::string(str));
}

void simulator::imgui::window::console::warn(const std::string& str) {
    appendRaw(t_warn, str);
}

void simulator::imgui::window::console::warn(const char* str) {
    warn(std::string(str));
}

void simulator::imgui::window::console::error(const std::string& str) {
    appendRaw(t_error, str);
}

void simulator::imgui::window::console::error(const char* str) {
    error(std::string(str));
}

void simulator::imgui::window::console::debug(const std::string& str) {
    appendRaw(t_debug, str);
}

void simulator::imgui::window::console::debug(const char* str) {
    debug(std::string(str));
}
