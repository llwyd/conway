#include "cycle.h"
#include "life_assert.h"

extern bool Cycle_Check8(const uint8_t * const data, uint32_t len)
{
    ASSERT(data != NULL);
    ASSERT(len > 0u);
    bool cycle_detected = false;

    return cycle_detected;
}

