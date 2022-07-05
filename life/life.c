#include "life.h"

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef enum {false, true} bool;

typedef struct
{
    unsigned char x;
    unsigned char y;
}
point_t;

/* Store cell format in same format as LCD display for ease */
static unsigned char ping_status [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };
static unsigned char pong_status [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };

static unsigned char (*ping)[LCD_COLUMNS];
static unsigned char (*pong)[LCD_COLUMNS];

void ( *update_fn )( void );

static uint8_t hash_table[ 256 ] =
{ 48, 242, 205, 49, 117, 39, 135, 200, 183, 15, 158, 175, 4, 192, 40, 199, 181, 77, 188, 70, 121, 119, 63, 73, 120, 216, 246, 69, 189, 11, 185, 85, 6, 93, 114, 232, 108, 5, 197, 196, 221, 132, 250, 148, 241, 195, 90, 36, 155, 176, 43, 252, 100, 227, 18, 88, 209, 208, 89, 35, 84, 37, 224, 16, 10, 194, 190, 22, 219, 55, 23, 177, 29, 154, 27, 106, 101, 13, 125, 92, 228, 152, 141, 30, 217, 237, 236, 31, 124, 239, 56, 145, 129, 33, 169, 161, 163, 184, 0, 180, 223, 47, 75, 3, 113, 62, 86, 142, 240, 134, 50, 249, 53, 20, 187, 248, 247, 66, 233, 254, 171, 131, 128, 52, 74, 9, 234, 72, 133, 67, 212, 127, 156, 7, 99, 61, 137, 206, 21, 34, 179, 57, 111, 26, 19, 231, 204, 1, 54, 98, 210, 110, 182, 193, 118, 17, 138, 165, 79, 112, 170, 139, 251, 2, 12, 122, 115, 28, 83, 203, 42, 214, 105, 167, 60, 178, 146, 225, 46, 64, 140, 174, 172, 143, 230, 32, 150, 160, 126, 220, 78, 116, 68, 213, 94, 229, 95, 147, 153, 104, 149, 162, 109, 201, 80, 51, 41, 136, 226, 144, 8, 96, 244, 235, 186, 218, 151, 243, 76, 103, 59, 198, 91, 87, 65, 24, 107, 157, 202, 102, 166, 82, 25, 123, 130, 81, 44, 238, 173, 58, 253, 255, 71, 168, 97, 164, 215, 159, 245, 191, 45, 38, 222, 14, 211, 207} ;


#define HASH_BUFFER_SIZE ( 8U )
static uint8_t hash_buffer[ HASH_BUFFER_SIZE];

static void DetermineSurroundingCells( point_t * cells, uint8_t x, uint8_t y );
static bool DetermineFate( bool alive, uint8_t num_alive );
static void Set( uint8_t (*life_cells)[LCD_COLUMNS], bool alive, uint8_t x_loc, uint8_t y_page, uint8_t y_bit );
static uint8_t CountLiveSurroundingCells( uint8_t (*life_cells)[LCD_COLUMNS], point_t * surrounding_cells );

unsigned char (*Life_GetBuffer( void ))[LCD_COLUMNS]
{
    return ping;
}

/* (Thanks wikipedia :) ) */
static unsigned int xorshift32( unsigned int x )
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
    uint8_t * slow = &hash_buffer[0];
    uint8_t * fast = &hash_buffer[2];
    bool match = false;

    for( int i = 2; i < HASH_BUFFER_SIZE; i+=2 )
    {
        if( *slow == *fast )
        {
            match = true;
            break;
        }
        slow++;
        fast++;
        fast++;
    }

   return match; 
}

void Life_Seed( void )
{
    ping = ping_status;
    pong = pong_status;
    static unsigned int seed = 0x12345678;    
    
    unsigned int rnd = xorshift32( seed );

    for( int i = 0; i < LCD_PAGES; i++ )
    {
        for( int j = 0; j < LCD_COLUMNS; j++ )
        {
            rnd = xorshift32( rnd );
            ping[i][j] = ( unsigned char )rnd;
            pong[i][j] = 0x00;
        }
    }

    for( int i = 0; i < HASH_BUFFER_SIZE; i++ )
    {
        rnd = xorshift32( rnd );
        hash_buffer[i] = (uint8_t)rnd;
    }

    seed = rnd;
}

void Life_Init( void ( *fn)( void ) )
{
    Life_Seed();
    update_fn = fn;
    update_fn();
}

static void DetermineSurroundingCells( point_t * cells, uint8_t x, uint8_t y )
{
    /* N */
    cells[ 0 ].x = x;
    cells[ 0 ].y = y - 1U;

    /* NE */
    cells[ 1 ].x = x + 1U;
    cells[ 1 ].y = y - 1U;
    
    /* E */
    cells[ 2 ].x = x + 1U;
    cells[ 2 ].y = y;
    
    /* SE */
    cells[ 3 ].x = x + 1U;
    cells[ 3 ].y = y + 1U;
    
    /* S */
    cells[ 4 ].x = x;
    cells[ 4 ].y = y + 1U;
    
    /* SW */
    cells[ 5 ].x = x - 1U;
    cells[ 5 ].y = y + 1U;
    
    /* W */
    cells[ 6 ].x = x - 1U;
    cells[ 6 ].y = y;
    
    /* NW */
    cells[ 7 ].x = x - 1U;
    cells[ 7 ].y = y - 1U;

    for( uint8_t idx = 0U; idx < 8; idx++ )
    {
        cells[idx].x = ( cells[idx].x & ( LCD_COLUMNS - 1U ) );
        cells[idx].y = ( cells[idx].y & ( LCD_FULL_ROWS - 1U ) );
    }
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

static void Set( uint8_t (*life_cells)[LCD_COLUMNS], bool alive, uint8_t x_loc, uint8_t y_page, uint8_t y_bit )
{    
    uint8_t bitmap  = ( 1U << y_bit );

    if( alive )
    {
        life_cells[y_page][x_loc] |= bitmap;
    }
    else
    {
        life_cells[y_page][x_loc] &= ~bitmap;
    }
}

static uint8_t CountLiveSurroundingCells( uint8_t (*life_cells)[LCD_COLUMNS], point_t * surrounding_cells )
{
    uint8_t num_alive = 0U;
    for( uint8_t idx = 0U; idx < 8U; idx++ )
    {
        uint8_t x_loc   = surrounding_cells[idx].x;
        uint8_t y_page  = surrounding_cells[idx].y >> 3;
        uint8_t y_bit   = surrounding_cells[idx].y & ( LCD_ROWS - 1U );

        uint8_t value = life_cells[y_page][x_loc];
        bool alive = (bool) ( (value >> y_bit) & 1U );
        if( alive )
        {
            num_alive++;
        }
    }
    
    return num_alive;
}

void Life_Tick( void )
{
    /* Surrounding cells */
    point_t cells [ 8 ];

    static uint8_t hash_counter = 0;
    uint8_t current_hash = 0;
    
    /* Go through each square, work out how many are alive */
    for( int i = 0; i < LCD_PAGES; i++ )
    {
        for( int j = 0; j < LCD_COLUMNS; j++ )
        {
            /* Go through each bit, check if alive, then work out which other bits to check */
            uint8_t current_value = ping[i][j];
            for( int k = 0; k < LCD_ROWS; k++ )
            {
                uint8_t current_bit = ( current_value >> k ) & 1U;
                bool alive = (bool) current_bit;

                point_t current_pos;
                current_pos.x = j;
                current_pos.y = k + (8 * i);
                DetermineSurroundingCells( cells, current_pos.x, current_pos.y );
                
                /* Determine how many of surrounding cells are alive */
                uint8_t num_alive = CountLiveSurroundingCells( ping, cells );
                bool fate = DetermineFate( alive, num_alive );

                Set( pong, fate, j, i, k );
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

    update_fn();

    if( CheckForCycle() )
    {
        Life_Init( update_fn );
    }
}
