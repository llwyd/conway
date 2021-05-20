#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define H_RES  ( 640 )
#define V_RES  ( 640 )


static Display * d;
static int screen;
static GC gc;

/* bitmap of whether cell is alive or dead */
static uint64_t status = 0x0;

void Life_Init( void )
{
    Display * d = XOpenDisplay( 0 );
    int screen = DefaultScreen( d );

    unsigned long black = BlackPixel( d, screen );
    unsigned long white = WhitePixel( d, screen );

    Window w = XCreateSimpleWindow(d, RootWindow( d, screen ), 0, 0, H_RES, V_RES, 1, white, black);
    XSetStandardProperties( d, w, "game of life", "game of life", None, NULL, 0, NULL );

    GC gc = XCreateGC( d, w, 0, 0 );

    XSetBackground( d, gc, black);
    XSetForeground( d, gc, white);
    XMapWindow( d, w);
    XFlush( d );
}


uint8_t main( void )
{
    Life_Init();

    //while(1);

    return 0;
}

