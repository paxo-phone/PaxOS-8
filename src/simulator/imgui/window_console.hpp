//
// Created by Charlito33 on 17/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#ifndef WINDOW_CONSOLE_HPP
#define WINDOW_CONSOLE_HPP

#include <imgui.h>
#include <string>
#include <vector>

namespace simulator::imgui::window::console {
    enum LogType {
        t_info,
        t_warn,
        t_error,
        t_debug
    };

    struct LogEntry {
        LogType type;
        std::string value;
    };

    extern ImU32 infoColor;
    extern ImU32 warnColor;
    extern ImU32 errorColor;
    extern ImU32 debugColor;

    extern bool autoScroll;
    extern std::vector<LogEntry *> lines;

    void render();

    void appendRaw(LogType type, const std::string& value);

    void info(const std::string& str);
    void info(const char *str);

    void warn(const std::string& str);
    void warn(const char *str);

    void error(const std::string& str);
    void error(const char *str);

    void debug(const std::string& str);
    void debug(const char *str);
}

#endif //WINDOW_CONSOLE_HPP
