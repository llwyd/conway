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
#include "life.h"
#include "bird.h"

static Display * d;
static int screen;
static Window w;
static GC gc;

static unsigned long black;
static unsigned long white;

void Init( void )
{
    d = XOpenDisplay( 0 );
    screen = DefaultScreen( d );

    black = BlackPixel( d, screen );
    white = WhitePixel( d, screen );

    w = XCreateSimpleWindow(d, RootWindow( d, screen ), 0, 0, LCD_COLUMNS * PIXEL_SIZE, LCD_ROWS * LCD_PAGES * PIXEL_SIZE, 1, black, white );
    XSetStandardProperties( d, w, "game of life", "game of life", None, NULL, 0, NULL );

    XSelectInput( d, w , ExposureMask | KeyPressMask | ButtonPressMask );

    gc = XCreateGC( d, w, 0, 0 );

    XSetBackground( d, gc, white );
    XSetForeground( d, gc, black );
    XMapWindow( d, w);
    XFlush( d );
}


void UpdateDisplay ( void )
{
#ifdef SIM_GOL
    unsigned char (*buffer)[LCD_COLUMNS] = Life_GetBuffer();
#else
    unsigned char (*buffer)[LCD_COLUMNS] = Bird_GetBuffer();
#endif

    for( int i = 0; i < LCD_PAGES; i++ )
    {
        for( int j = 0; j < LCD_COLUMNS; j++ )
        {
            for( int k = 0; k < LCD_ROWS; k++ )
            {
                bool alive = (bool)( ( buffer[i][j] >> k ) & 0x1 );
                uint8_t x_loc = j;
                uint8_t y_loc = k + (8 * i );

                if( alive )
                {
                    XSetForeground( d, gc, black );
                    XFillRectangle( d, w, gc, x_loc * PIXEL_SIZE, y_loc * PIXEL_SIZE, 1 * PIXEL_SIZE, 1 * PIXEL_SIZE );
                }
                else
                {
                    XSetForeground( d, gc, white );
                    XFillRectangle( d, w, gc, x_loc * PIXEL_SIZE, y_loc * PIXEL_SIZE, 1 * PIXEL_SIZE , 1 * PIXEL_SIZE );
                }
            }
        }
    }
}



int main( void )
{
    XEvent e;
    bool running = false;
    
    Init();
#ifdef SIM_GOL
    Life_Init( &UpdateDisplay, 0x12345678 );
#else
    Bird_Init( &UpdateDisplay, 0x12345678 );
#endif
    XNextEvent( d, &e );


    while(1)
    {
        if( XPending( d ) )
        {
            XNextEvent( d, &e );
            switch( e.type )
            {
                case ButtonPress:
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
#ifdef SIM_GOL
                Life_Tick();
#else
                Bird_Tick();
#endif
                usleep(25000);
            }
        }

        usleep( 50000 );
    }

    return 0;
}

