#include "cycle.h"
#include "life_assert.h"

/* Slow and steady wins the race... */

extern bool Cycle_Check8(const uint8_t * const data, uint32_t start_idx, uint32_t len)
{
    ASSERT(data != NULL);
    ASSERT(len > 0u);
    ASSERT(start_idx < len);
    /* Must be power of 2 for the bit-modulo */
    ASSERT((len & (len - 1)) == 0U);

    bool cycle_detected = false;
    bool first_match = false;
    bool second_match = false;
    
    uint32_t tort = start_idx + 1u;
    tort = tort & ( len - 1U );
    
    uint32_t hare = tort + 1u;
    hare = hare & ( len - 1U );
    
    uint8_t match_idx = 0;

    /* 1. Tortoise moves single step, hare moves double step */
    for( uint8_t idx = 2; idx < len; idx+=2 )
    {
        if( data[tort] == data[hare] )
        {
            first_match = true;
            match_idx = idx;
            break;
        }

        tort++;
        hare+=2;
        
        tort = tort & ( len - 1U );
        hare = hare & ( len - 1U );
    }

    if( first_match )
    {
        /* 2. Move Tortoise back to start and move both single step */
        tort = start_idx;
        for( uint8_t idx = match_idx; idx < len; idx++ )
        {
            if( data[tort] == data[hare] )
            {
                second_match = true;
                break;
            }

            tort++;
            hare++;
        
            tort = tort & ( len - 1U );
            hare = hare & ( len - 1U );
        }
  
        if( second_match )
        {
            /* 3. Move Hare to tortoise, single step hare */

            hare = tort + 1;
            hare = hare & ( len - 1U );

            for( uint8_t idx = 0; idx < len; idx++ )
            {
                if( data[tort] == data[hare] )
                {
                    cycle_detected = true;
                    break;
                }
                hare++;
                hare = hare & ( len - 1U );

            }
        }
    }
    
    return cycle_detected;
}

