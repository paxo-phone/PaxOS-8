#include "time.hpp"

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include <chrono>
#include <cstdint>

uint64_t micros(void)
{
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - 1677588060000000;
}

uint64_t millis(void)
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - 1677588060000;
}

void delay(uint64_t ms)
{
    uint64_t start = millis();
    while (millis() - start < ms);
}

#endif /* #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) */