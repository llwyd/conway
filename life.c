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

    w = XCreateSimpleWindow(d, RootWindow( d, screen ), 0, 0, H_RES, V_RES, 1, white, black);
    XSetStandardProperties( d, w, "game of life", "game of life", None, NULL, 0, NULL );

    XSelectInput( d, w , ExposureMask | KeyPressMask | ButtonPressMask );

    gc = XCreateGC( d, w, 0, 0 );

    XSetBackground( d, gc, black);
    XSetForeground( d, gc, white);
    XMapWindow( d, w);
    XFlush( d );
}

void Life_Click( int x, int y )
{
    printf("Mouse Click\n");
    printf("Clicked at %d, %d\n", x, y );
   
    XFillRectangle( d, w, gc, x, y, 80, 80 );
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

