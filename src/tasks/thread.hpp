#ifndef THREAD_HPP
#define THREAD_HPP

#include "../includes.h"

#define CORE_APP 0
#define CORE_BACK 1

#ifdef BUILD_EMU

#include <thread>

#endif

void new_thread(bool core, void(*func)(void*), void* arg);

#endif
