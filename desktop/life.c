#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "assert.h"

/* These parameters are for the SSD1306 Display */
#define LCD_COLUMNS (128 )
#define LCD_ROWS    ( 8 )
#define LCD_PAGES   ( 8)

typedef struct
{
    uint8_t x;
    uint8_t y;
}
point_t;

/* Store cell format in same format as LCD display for ease */
uint8_t ping_status [ LCD_COLUMNS ] [ LCD_PAGES ] = { 0x00 };
uint8_t pong_status [ LCD_COLUMNS ] [ LCD_PAGES ] = { 0x00 };

uint8_t (*ping)[LCD_PAGES];
uint8_t (*pong)[LCD_PAGES];

static Display * d;
static int screen;
static Window w;
static GC gc;

unsigned long black;
unsigned long white;

/* bitmap of whether cell is alive or dead */
static uint64_t status = 0x0;

uint64_t Life_CalculateLiveBits( int x, int y );
uint64_t Life_Coordinate2Bit64( int x, int y);


/* (Thanks wikipedia :) ) */
uint32_t xorshift32( uint32_t x )
{
    x ^= x << 13U;
    x ^= x >> 17U;
    x ^= x << 5U;

    return x;
}


void Life_Init( void )
{
    d = XOpenDisplay( 0 );
    screen = DefaultScreen( d );

    black = BlackPixel( d, screen );
    white = WhitePixel( d, screen );

    w = XCreateSimpleWindow(d, RootWindow( d, screen ), 0, 0, LCD_COLUMNS, LCD_ROWS * LCD_PAGES, 1, black, white );
    XSetStandardProperties( d, w, "game of life", "game of life", None, NULL, 0, NULL );

    XSelectInput( d, w , ExposureMask | KeyPressMask | ButtonPressMask );

    gc = XCreateGC( d, w, 0, 0 );

    XSetBackground( d, gc, white );
    XSetForeground( d, gc, black );
    XMapWindow( d, w);
    XFlush( d );

    ping = ping_status;
    pong = pong_status;

    uint32_t seed = 0x12345678;
    uint32_t rnd = xorshift32( seed );

    for( int i = 0; i < LCD_COLUMNS; i++ )
    {
        for( int j = 0; j < LCD_PAGES; j++ )
        {
            rnd = xorshift32( rnd );
            ping[i][j] = (uint8_t)rnd;
        }
    }
}

void Life_DetermineSurroundingCells( point_t * cells, uint8_t x, uint8_t y )
{
    memset( cells, 0x00, 8U );
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

void Life_Click( int x, int y )
{
    printf("Mouse Click\n");
    printf("Clicked at %d, %d\n", x, y );
 
    
    XSetForeground( d, gc, black );
    XFillRectangle( d, w, gc, x, y, 1, 1 );
   
    /* Set bit in cell status */
    uint8_t x_loc = x;
    uint8_t y_page = y >> 3;
    uint8_t y_bit   = y % LCD_ROWS;

    ping[ x ][ y_page ] = ( 0x1 << y_bit );   

    printf("x_loc: %d\n", x_loc);
    printf("y_page: %d\n", y_page);
    printf("y_bit: %d\n", y_bit);
    
    /* Surrounding cells */
    point_t cells [ 8 ];
    Life_DetermineSurroundingCells( cells, x, y );
}

bool Life_DetermineFate( bool alive, uint8_t num_alive )
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

void Life_Set( uint8_t (*life_cells)[LCD_PAGES], bool alive, point_t current_pos )
{    
    uint8_t x_loc   = current_pos.x;
    uint8_t y_page  = current_pos.y >> 3;
    uint8_t y_bit   = current_pos.y % LCD_ROWS;
    
    uint8_t bitmap  = ( 1U << y_bit );

    if( alive )
    {
        XSetForeground( d, gc, black );
        XFillRectangle( d, w, gc, current_pos.x, current_pos.y, 1, 1 );
        
        life_cells[x_loc][y_page] |= bitmap;
    }
    else
    {
        XSetForeground( d, gc, white );
        XFillRectangle( d, w, gc, current_pos.x, current_pos.y, 1, 1 ); 
        
        life_cells[x_loc][y_page] &= ~bitmap;
    }
}

uint8_t Life_CountLiveSurroundingCells( uint8_t (*life_cells)[LCD_PAGES], point_t * surrounding_cells )
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
            for( int k = 0; k < LCD_ROWS; k ++ )
            {
                uint8_t current_bit = ( current_value >> k ) & 1U;
                bool alive = (bool) current_bit;

                point_t current_pos;
                current_pos.x = i;
                current_pos.y = k + (8 * j);
                Life_DetermineSurroundingCells( cells, current_pos.x, current_pos.y );
                
                /* Determine how many of surrounding cells are alive */
                uint8_t num_alive = Life_CountLiveSurroundingCells( ping, cells );
                bool fate = Life_DetermineFate( alive, num_alive );

                Life_Set( pong, fate, current_pos );
            }    
        } 
    }

    /* Swap pointers for ping pong buffer */
    uint8_t (*temp)[LCD_PAGES] = ping;
    ping = pong;
    pong = temp;
}

uint8_t main( void )
{
    XEvent e;
    Life_Init();
    XNextEvent( d, &e );

    bool running = false;

    while(1)
    {
        if( XPending( d ) )
        {
            XNextEvent( d, &e );
            switch( e.type )
            {
                case ButtonPress:
                    Life_Click( e.xbutton.x, e.xbutton.y );
                    //Life_Tick();
                    break;
                case KeyPress:
                    if( running )
                    {
                        running = false;
                    }
                    else
                    {
                        running = true;
                    }
                    printf("Running: %d\n", running);
                    break;
                default:
                    printf("Unhandled Event\n");
                    break;
            }

        }
        else
        {
            if( running )
            {
                Life_Tick();
                usleep(1000 * 250);
            }
        }

        usleep( 1000 );
    }

    return 0;
}

