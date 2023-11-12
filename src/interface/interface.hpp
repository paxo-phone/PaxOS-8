#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#ifdef BUILD_EMU
  #include <chrono>
  #include <cstdlib>

  uint64_t micros();

  uint64_t millis();

  void delay(uint64_t ms);

  int random(int i, int j);
#endif

#include "filestream.hpp"
#include "filetree.hpp"
#include "shell.hpp"
#include "GSM/GSM.hpp"
#include "button/button.hpp"
#include "lights.hpp"

#endif
