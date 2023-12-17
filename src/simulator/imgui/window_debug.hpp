//
// Created by Charlito33 on 16/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#ifndef PAXOS_WINDOW_DEBUG_HPP
#define PAXOS_WINDOW_DEBUG_HPP

namespace simulator::imgui::window::debug {
    // Sim800L values
    extern int batteryLevel;
    extern int networkQuality;

    extern bool automaticDate;
    extern int dateYears;
    extern int dateMonths;
    extern int dateDays;
    extern int dateHours;
    extern int dateMinutes;
    extern int dateSeconds;

    void render();
}

#endif //PAXOS_WINDOW_DEBUG_HPP
