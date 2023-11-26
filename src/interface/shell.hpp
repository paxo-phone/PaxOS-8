#ifndef SHELL_HPP
#define SHELL_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include "filestream.hpp"
#include "console.hpp"

#include "../tasks/tasks.hpp"

namespace shell
{
    void thread_shell(void* data);
}


#endif /* SHELL_HPP */