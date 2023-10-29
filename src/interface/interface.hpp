#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#ifdef BUILD_EMU
  #include <chrono>

  uint64_t micros() {
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - 1677588060000000;
  }

  uint64_t millis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - 1677588060000;
  }

  void delay(uint64_t ms)
  {
    uint64_t start = millis();
    while (millis() - start < ms);
  }

  int random(int i, int j)
  {
    return (rand()%(j-i))-i;
  }
#endif

#include "filestream.hpp"
#include "filetree.hpp"
#include "shell.hpp"
#include "GSM/GSM.hpp"
#include "button/button.hpp"
#include "lights.hpp"

#endif
