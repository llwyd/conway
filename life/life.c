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
static unsigned char ping_status [ LCD_COLUMNS ] [ LCD_PAGES ] = { 0x00 };
static unsigned char pong_status [ LCD_COLUMNS ] [ LCD_PAGES ] = { 0x00 };

static unsigned char (*ping)[LCD_PAGES];
static unsigned char (*pong)[LCD_PAGES];

static void DetermineSurroundingCells( point_t * cells, uint8_t x, uint8_t y );
static bool DetermineFate( bool alive, uint8_t num_alive );
static void Set( uint8_t (*life_cells)[LCD_PAGES], bool alive, uint8_t x_loc, uint8_t y_page, uint8_t y_bit );
static uint8_t CountLiveSurroundingCells( uint8_t (*life_cells)[LCD_PAGES], point_t * surrounding_cells );
static void ClearBuffer( uint8_t (*life_cells)[LCD_PAGES] );


unsigned char (*Life_GetBuffer( void ))[LCD_PAGES]
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


void Life_Init( void )
{
    ping = ping_status;
    pong = pong_status;
    
    unsigned int seed = 0x12345678;
    unsigned int rnd = xorshift32( seed );

    for( int i = 0; i < LCD_COLUMNS; i++ )
    {
        for( int j = 0; j < LCD_PAGES; j++ )
        {
            rnd = xorshift32( rnd );
            ping[i][j] = ( unsigned char )rnd;
            pong[i][j] = 0x00;
        }
    }

}

static void ClearBuffer( uint8_t (*life_cells)[LCD_PAGES] )
{
    for( int i = 0; i < LCD_COLUMNS; i++ )
    {
        for( int j = 0; j < LCD_PAGES; j++ )
        {
            life_cells[i][j] = 0x00;
        }
    }
}

static void DetermineSurroundingCells( point_t * cells, uint8_t x, uint8_t y )
{

    for( uint8_t idx = 0; idx < 8U; idx++ )
    {
        cells[ idx ].x = 0x00;
        cells[ idx ].y = 0x00;
    }

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
        cells[idx].x %= LCD_COLUMNS;
        cells[idx].y %= LCD_ROWS * LCD_PAGES;
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

static void Set( uint8_t (*life_cells)[LCD_PAGES], bool alive, uint8_t x_loc, uint8_t y_page, uint8_t y_bit )
{    
    uint8_t bitmap  = ( 1U << y_bit );

    if( alive )
    {
        life_cells[x_loc][y_page] |= bitmap;
    }
    else
    {
        life_cells[x_loc][y_page] &= ~bitmap;
    }
}

static uint8_t CountLiveSurroundingCells( uint8_t (*life_cells)[LCD_PAGES], point_t * surrounding_cells )
{
    uint8_t num_alive = 0U;
    for( uint8_t idx = 0U; idx < 8U; idx++ )
    {
        uint8_t x_loc   = surrounding_cells[idx].x;
        uint8_t y_page  = surrounding_cells[idx].y >> 3;
        uint8_t y_bit   = surrounding_cells[idx].y % LCD_ROWS;

        uint8_t value = life_cells[x_loc][y_page];
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
    
    /* Go through each square, work out how many are alive */
    for( int i = 0; i < LCD_COLUMNS; i++ )
    {
        for( int j = 0; j < LCD_PAGES; j++ )
        {
            /* Go through each bit, check if alive, then work out which other bits to check */
            uint8_t current_value = ping[i][j];
            for( int k = 0; k < LCD_ROWS; k++ )
            {
                uint8_t current_bit = ( current_value >> k ) & 1U;
                bool alive = (bool) current_bit;

                point_t current_pos;
                current_pos.x = i;
                current_pos.y = k + (8 * j);
                DetermineSurroundingCells( cells, current_pos.x, current_pos.y );
                
                /* Determine how many of surrounding cells are alive */
                uint8_t num_alive = CountLiveSurroundingCells( ping, cells );
                bool fate = DetermineFate( alive, num_alive );

                Set( pong, fate, i, j, k );
            }    
        } 
    }

    /* Swap pointers for ping pong buffer */
    uint8_t (*temp)[LCD_PAGES] = ping;
    ping = pong;
    pong = temp;

}
