#include "random.hpp"

#ifdef BUILD_EMU

#include <cstdint>
#include <cstdlib>

int16_t random(const int16_t start, const int16_t end)
{
    return (rand() % (end - start));
}

#endif /* BUILD_EMU */