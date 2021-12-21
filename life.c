#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

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
uint8_t cell_status [ LCD_COLUMNS ] [ LCD_PAGES ] = { 0x00 };

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
}

void Life_DetermineSurroundingCells( uint8_t x, uint8_t y )
{
    /* Surrounding cells */
    point_t cells [ 8 ];

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

    printf("Surrounding Cells for %d,%d:\n", x, y);

    for( uint8_t idx = 0U; idx < 8; idx++ )
    {
        cells[idx].x %= LCD_COLUMNS;
        cells[idx].y %= LCD_ROWS * LCD_PAGES;
        printf("[%d]: %d,%d\n", idx, cells[idx].x, cells[idx].y);
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

    cell_status[ x ][ y_page ] = ( 0x1 << y_bit );   

    printf("x_loc: %d\n", x_loc);
    printf("y_page: %d\n", y_page);
    printf("y_bit: %d\n", y_bit);
    
    Life_DetermineSurroundingCells( x, y );
}

uint64_t Life_Coordinate2Bit64( int x, int y)
{
    uint64_t bit = ( 0x1 << x );
    bit = bit << ( y * 8 );
    return bit;
}

bool Life_IsCellAlive( uint64_t x )
{
    bool alive = ((x & status) == x);
    return alive;
}

/* This function calculates the bit mask of bits to check for a given bit */
uint64_t Life_CalculateLiveBits( int x, int y )
{
    uint64_t current_bitmask = 0U;
    /* Create bit mask */
    uint64_t start_x = ( 0x1 << x );
    uint64_t start_mid = start_x << ( y * 8 );

    
    /* Cant shift more than 32 bits */ 
    uint64_t mask_mid = 0xFF;
    for(int i = 0; i < y; i++)
    {
        mask_mid <<= 8U;
    }

    uint64_t mask_blw = ( mask_mid >> 8 );
    uint64_t mask_abv = ( mask_mid << 8 );

    uint64_t start_abv = start_mid << ( 8 );
    uint64_t start_blw = start_mid >> ( 8 ); 
            
    current_bitmask |= ( ( start_mid << 1 ) & mask_mid );
    current_bitmask |= ( ( start_mid >> 1 ) & mask_mid );
            
    current_bitmask |= ( start_abv );
    current_bitmask |= ( ( start_abv << 1 ) & mask_abv );
    current_bitmask |= ( ( start_abv >> 1 ) & mask_abv );
            
    current_bitmask |= ( start_blw );
    current_bitmask |= ( ( start_blw << 1 ) & mask_blw );
    current_bitmask |= ( ( start_blw >> 1 ) & mask_blw );

    return current_bitmask;
}

bool Life_DetermineFate( bool alive, uint64_t mask )
{
   bool fate = false;
   uint64_t current_live = status & mask;
   uint32_t num_alive = __builtin_popcountll( current_live );
   //printf("num_alive = %d\n", num_alive);
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

void Life_Set( int true_x, int true_y, bool alive, uint64_t * next_status )
{
    
    uint64_t shift_x = 0x1 << true_x;
    uint64_t shift_y = shift_x << ( true_y * 8 );

    int x = true_x * 8 * 10;
    int y = true_y * 8 * 10;

    if( alive )
    {
        XSetForeground( d, gc, black );
        XFillRectangle( d, w, gc, x, y, 80, 80 );
        *next_status |= shift_y;
    }
    else
    {
        XSetForeground( d, gc, white );
        XFillRectangle( d, w, gc, x, y, 80, 80 ); 
        *next_status &= ~shift_y;
    }

    uint16_t val = true_x + ( true_y * 8);

    printf("Val: %d\n", val); 
    printf("Bit : 0x%" PRIx64 "\n", shift_y);
    printf("Live: 0x%" PRIx64 "\n", status);
    
    uint64_t live_mask = Life_CalculateLiveBits( true_x, true_y);
    printf("LiveMask: 0x%" PRIx64 "\n", live_mask );

}

void Life_Tick( void )
{
    /* Go through each square, work out how many are alive */
    uint64_t next_status = 0U;
    for( int i = 0; i < 8; i++ )
    {
        for( int j = 0; j < 8; j++ )
        {
            uint64_t current_cell = Life_Coordinate2Bit64( j, i );
            bool currently_alive = Life_IsCellAlive( current_cell );
            uint64_t current_mask = Life_CalculateLiveBits( j, i );
            
            bool next_state = Life_DetermineFate( currently_alive, current_mask);

            Life_Set( j, i, next_state, &next_status );
            printf("Cell (%d,%d) status = %d next_state = %d\n",j,i, currently_alive, next_state);
        } 
    }
    status = next_status;
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

