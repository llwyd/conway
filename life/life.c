#include "life.h"

/* If violated there be dragons, code works on assumption of 8bit storage */
_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define CONSECUTIVE_CYCLES ( 64U )
#define HASH_BUFFER_SIZE ( 16U )
#define SURROUNDING_CELLS ( 8U )
typedef struct
{
    uint8_t x;
    uint8_t y;
}
point_t;

typedef struct
{
    uint8_t col;
    uint8_t page;
    uint8_t bit;
}
bit_t;

static bool DetermineFate( bool alive, uint8_t num_alive );
static void Set( uint8_t (* const life_cells)[LCD_COLUMNS], bool alive, const bit_t * const bit );
static uint8_t CountLiveSurroundingCells(const point_t * const point,uint8_t (*life_cells)[LCD_COLUMNS]);

static bit_t PointToBit(const point_t * const point);
static point_t BitToPoint(const bit_t * const bit);

/* Store cell format in same format as LCD display for ease */
static uint8_t ping_status [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };
static uint8_t pong_status [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };

static uint8_t (*ping)[LCD_COLUMNS];
static uint8_t (*pong)[LCD_COLUMNS];

static const point_t s_cells[SURROUNDING_CELLS] =
{
    /* E */
    { .x = 1U, .y = 0U },
    /* SE */
    { .x = 1U, .y = 1U },
    /* S */
    { .x = 0U, .y = 1U },
    /* SW */
    { .x = UINT8_MAX, .y = 1U },
    /* W */
    { .x = UINT8_MAX, .y = 0U },
    /* NW */
    { .x = UINT8_MAX, .y = UINT8_MAX },
    /* N */
    { .x = 0U, .y = UINT8_MAX },
    /* NE */
    { .x = 1U, .y = UINT8_MAX },
};

static const uint8_t hash_table[ 256 ] =
{ 48, 242, 205, 49, 117, 39, 135, 200, 183, 15, 158, 175, 4, 192, 40, 199, 181, 77, 188, 70, 121, 119, 63, 73, 120, 216, 246, 69, 189, 11, 185, 85, 6, 93, 114, 232, 108, 5, 197, 196, 221, 132, 250, 148, 241, 195, 90, 36, 155, 176, 43, 252, 100, 227, 18, 88, 209, 208, 89, 35, 84, 37, 224, 16, 10, 194, 190, 22, 219, 55, 23, 177, 29, 154, 27, 106, 101, 13, 125, 92, 228, 152, 141, 30, 217, 237, 236, 31, 124, 239, 56, 145, 129, 33, 169, 161, 163, 184, 0, 180, 223, 47, 75, 3, 113, 62, 86, 142, 240, 134, 50, 249, 53, 20, 187, 248, 247, 66, 233, 254, 171, 131, 128, 52, 74, 9, 234, 72, 133, 67, 212, 127, 156, 7, 99, 61, 137, 206, 21, 34, 179, 57, 111, 26, 19, 231, 204, 1, 54, 98, 210, 110, 182, 193, 118, 17, 138, 165, 79, 112, 170, 139, 251, 2, 12, 122, 115, 28, 83, 203, 42, 214, 105, 167, 60, 178, 146, 225, 46, 64, 140, 174, 172, 143, 230, 32, 150, 160, 126, 220, 78, 116, 68, 213, 94, 229, 95, 147, 153, 104, 149, 162, 109, 201, 80, 51, 41, 136, 226, 144, 8, 96, 244, 235, 186, 218, 151, 243, 76, 103, 59, 198, 91, 87, 65, 24, 107, 157, 202, 102, 166, 82, 25, 123, 130, 81, 44, 238, 173, 58, 253, 255, 71, 168, 97, 164, 215, 159, 245, 191, 45, 38, 222, 14, 211, 207} ;

static uint8_t hash_buffer[ HASH_BUFFER_SIZE];
static uint8_t hash_counter = 0;

static uint32_t seed; 
static uint32_t original_seed;

extern uint8_t (*Life_GetBuffer( void ))[LCD_COLUMNS]
{
    return ping;
}

/* (Thanks wikipedia :) ) */
static uint32_t xorshift32( uint32_t x )
{
    x ^= x << 13U;
    x ^= x >> 17U;
    x ^= x << 5U;

    return x;
}

static uint8_t UpdateHash( uint8_t current_hash, uint8_t value )
{
    uint8_t index = current_hash ^ value;
    return hash_table[index];
}

bool CheckForCycle( void )
{
    bool firstMatch = false;
    bool secondMatch = false;
    bool thirdMatch = false;

    bool cycleDetected = false;
    bool startNewGame = false;

    /* Number of consecutive times a cycle is detected */
    static uint8_t cycleDetectedCount = 0U;

    /* Tortoise and Hare algorithm to detect cycle */
    uint8_t tort = hash_counter + 1;
    uint8_t hare = tort + 1;

    tort = tort & ( HASH_BUFFER_SIZE - 1U );
    hare = hare & ( HASH_BUFFER_SIZE - 1U );

    uint8_t mu = 0U;
    uint8_t lam = 1;

    uint8_t firstCount = 0;

    /* 1. Tortoise moves single step, hare moves double step */
    for( uint8_t idx = 2; idx < HASH_BUFFER_SIZE; idx+=2 )
    {
        if( hash_buffer[tort] == hash_buffer[hare] )
        {
            firstMatch = true;
            firstCount = idx;
            break;
        }

        tort++;
        hare+=2;
        
        tort = tort & ( HASH_BUFFER_SIZE - 1U );
        hare = hare & ( HASH_BUFFER_SIZE - 1U );
    }

    if( firstMatch )
    {
        /* 2. Move Tortoise back to start and move both single step */
        tort = hash_counter;
        for( uint8_t idx = firstCount; idx < HASH_BUFFER_SIZE; idx++ )
        {
            if( hash_buffer[tort] == hash_buffer[hare] )
            {
                secondMatch = true;
                break;
            }

            tort++;
            hare++;
        
            tort = tort & ( HASH_BUFFER_SIZE - 1U );
            hare = hare & ( HASH_BUFFER_SIZE - 1U );

            mu++;
        }
  
        if( secondMatch )
        {
            /* 3. Move Hare to tortoise, single step hare */

            hare = tort + 1;
            hare = hare & ( HASH_BUFFER_SIZE - 1U );

            for( uint8_t idx = 0; idx < HASH_BUFFER_SIZE; idx++ )
            {
                if( hash_buffer[tort] == hash_buffer[hare] )
                {
                    thirdMatch = true;
                    break;
                }
                hare++;
                hare = hare & ( HASH_BUFFER_SIZE - 1U );

                lam++;
            }

            if( thirdMatch )
            {
                cycleDetected = true;
            }
        }
    }

    if( cycleDetected )
    {
        cycleDetectedCount++;
        
        if( cycleDetectedCount > CONSECUTIVE_CYCLES )
        {
            startNewGame = true;
            cycleDetectedCount = 0U;
        }
    }
    else
    {
        cycleDetectedCount = 0U;
    }

   return startNewGame;
}

extern void Life_Seed( void )
{
    ping = ping_status;
    pong = pong_status;

    /* This is to diagnose and reproduce bugs :) */
    original_seed = seed;

    uint32_t rnd = xorshift32( seed );

    for( uint8_t i = 0; i < LCD_PAGES; i++ )
    {
        for( uint8_t j = 0; j < LCD_COLUMNS; j++ )
        {
            rnd = xorshift32( rnd );
            ping[i][j] = ( uint8_t )rnd;
            pong[i][j] = 0x00;
        }
    }

    for( uint8_t i = 0; i < HASH_BUFFER_SIZE; i++ )
    {
        rnd = xorshift32( rnd );
        hash_buffer[i] = (uint8_t)rnd;
    }

    seed = rnd;
}

extern void Life_Init( uint32_t initial_seed )
{
    seed = initial_seed;
    Life_Seed();
}

static bit_t PointToBit(const point_t * const point)
{
    bit_t bit =
    {
        .col   = point->x,
        .page  = point->y >> 3,
        .bit   = point->y & ( LCD_ROWS - 1U ),
    };

    return bit;
}

static point_t BitToPoint(const bit_t * const bit)
{
    point_t point = 
    {
        .x = bit->col,
        .y = bit->bit + (8 * bit->page),
    };

    return point;
}

static bool DetermineFate( bool alive, uint8_t num_alive )
{
   bool fate = false;
   
   if( alive )
   {
       /* Rule 1,2 and 3. */
       if( ( num_alive < 2 ) || ( num_alive > 3 ) )
       {
            fate = false;
       }
       else
       {
            fate = true;
       }
   }
   else
   {
        /* Rule 4. */
        if( num_alive == 3U)
        {
            fate = true;
        }
   }

   return fate; 
}

static void Set( uint8_t (* const life_cells)[LCD_COLUMNS], bool alive, const bit_t * const bit )
{    
    if( alive )
    {
        life_cells[bit->page][bit->col] |= ( 1U << bit->bit);
    }
    else
    {
        life_cells[bit->page][bit->col] &= ~(1U << bit->bit);
    }
}

static uint8_t CountLiveSurroundingCells(const point_t * const point, uint8_t (*life_cells)[LCD_COLUMNS])
{
    uint8_t num_alive = 0U;
    for( uint8_t idx = 0U; idx < 8U; idx++ )
    {
        const point_t p =
        {
            .x = (( point->x + s_cells[idx].x) & ( LCD_COLUMNS - 1U )),
            .y = (( point->y + s_cells[idx].y) & ( LCD_FULL_ROWS - 1U )),
        };

        const bit_t bit = PointToBit(&p);

        uint8_t value = life_cells[bit.page][bit.col];
        bool alive = (bool) ( (value >> bit.bit) & 1U );
        if( alive )
        {
            num_alive++;
        }
    }
    
    return num_alive;
}

extern void Life_Tick( void )
{
    uint8_t current_hash = 0;
    
    /* Go through each square, work out how many are alive */
    for( uint8_t i = 0; i < LCD_PAGES; i++ )
    {
        for( uint8_t j = 0; j < LCD_COLUMNS; j++ )
        {
            /* Go through each bit, check if alive, then work out which other bits to check */
            for( uint8_t k = 0; k < LCD_ROWS; k++ )
            {
                const bit_t current_bit = 
                {
                    .col = j,
                    .page = i,
                    .bit = k,
                };

                bool alive = (bool)((ping[i][j] >> k )& 1U);

                const point_t current_pos = BitToPoint(&current_bit);
                
                /* Determine how many of surrounding cells are alive */
                const uint8_t num_alive = CountLiveSurroundingCells(&current_pos, ping);
                const bool fate = DetermineFate( alive, num_alive );

                Set( pong, fate, &current_bit );
            }
            current_hash = UpdateHash( current_hash, pong[i][j] ); 
        }   
    }

    hash_buffer[hash_counter] = current_hash;
    hash_counter++;
    hash_counter = ( hash_counter & ( HASH_BUFFER_SIZE - 1U ) );

    /* Swap pointers for ping pong buffer */
    uint8_t (*temp)[LCD_COLUMNS] = ping;
    ping = pong;
    pong = temp;

    if( CheckForCycle() )
    {
        Life_Init( seed );
    }
}
