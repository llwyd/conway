#include "cycle.h"
#include "life_assert.h"

extern bool Cycle_Check8(const uint8_t * const data, uint32_t len)
{
    ASSERT(data != NULL);
    ASSERT(len > 0u);
    bool cycle_detected = false;
    bool first_match = false;
    bool second_match = false;
    bool third_match = false;
    
    uint32_t tort = 0;
    uint32_t hare = 1;
    
    /* 1. Tortoise moves single step, hare moves double step */
    uint8_t firstCount = 0;
    for( uint8_t idx = 2; idx < len; idx+=2, tort++, hare+=2 )
    {
        if( data[tort] == data[hare] )
        {
            first_match = true;
            firstCount = idx;
            break;
        }
    }
    
    if( first_match )
    {
        /* 2. Move Tortoise back to start and move both single step */
        tort = 0;
        for( uint8_t idx = firstCount; idx < len; idx++, tort++, hare++ )
        {
            if(hare > len)
            {
                hare = 0;
            }
            if( data[tort] == data[hare] )
            {
                second_match = true;
                break;
            }
        }
  
        if( second_match )
        {
            /* 3. Move Hare to tortoise, single step hare */
            hare = tort + 1;

            for( uint8_t idx = 0; idx < len; idx++, hare++ )
            {
                if( data[tort] == data[hare] )
                {
                    third_match = true;
                    break;
                }
            }

            if( third_match )
            {
                cycle_detected = true;
            }
        }
    }

    return cycle_detected;
}

