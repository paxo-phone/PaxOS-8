#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "memory.hpp"
#include "shell.hpp"

#ifdef BUILD_EMU
  #include <chrono>

  uint64_t micros() {
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
  }

  uint64_t millis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  }

  void delay(uint64_t ms)
  {
    uint64_t start = millis();
    while (millis() - start < ms);
  }
#endif

#endif