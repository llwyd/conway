#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define H_RES  ( 640 )
#define V_RES  ( 640 )

/* 8x8 grid for game of life */

static Display * d;
static int screen;
static Window w;
static GC gc;

/* bitmap of whether cell is alive or dead */
static uint64_t status = 0x0;

void Life_Init( void )
{
    d = XOpenDisplay( 0 );
    screen = DefaultScreen( d );

    unsigned long black = BlackPixel( d, screen );
    unsigned long white = WhitePixel( d, screen );

    w = XCreateSimpleWindow(d, RootWindow( d, screen ), 0, 0, H_RES, V_RES, 1, black, white );
    XSetStandardProperties( d, w, "game of life", "game of life", None, NULL, 0, NULL );

    XSelectInput( d, w , ExposureMask | KeyPressMask | ButtonPressMask );

    gc = XCreateGC( d, w, 0, 0 );

    XSetBackground( d, gc, white );
    XSetForeground( d, gc, black );
    XMapWindow( d, w);
    XFlush( d );
}

void Life_Click( int x, int y )
{
    printf("Mouse Click\n");
    printf("Clicked at %d, %d\n", x, y );
 
    x/=10;
    y/=10;

    uint8_t true_x = ( uint8_t )( x >> 3 );
    uint8_t true_y = ( uint8_t )( y >> 3 );

    printf("%d,%d\n", true_x, true_y );

    x = true_x * 8 * 10;
    y = true_y * 8 * 10;

    XFillRectangle( d, w, gc, x, y, 80, 80 );

    uint64_t shift_x = 0x1 << true_x;
    uint64_t shift_y = shift_x << ( true_y * 8 );

    status |= shift_y;

    printf("Live: 0x%" PRIx64 "\n", status);
}

void Life_Tick( void )
{

}

uint8_t main( void )
{
    XEvent e;
    Life_Init();
    XNextEvent( d, &e );

    while(1)
    {
        if( XPending( d ) )
        {
            XNextEvent( d, &e );
            switch( e.type )
            {
                case ButtonPress:
                    Life_Click( e.xbutton.x, e.xbutton.y );
                    break;
                default:
                    printf("Unhandled Event\n");
                    break;
            }

        }
        else
        {

        }

        usleep( 1000 );
    }

    return 0;
}

