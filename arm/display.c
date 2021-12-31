
#include "i2c.h"

static void Configure( unsigned char command )
{
    unsigned char commands[2] = { 0x00, 0x00};
    unsigned char address = 0x3C;

    commands[1] = command;
    I2C_Write( address, commands, 2 );
}

static void ConfigureWithData( unsigned char command, unsigned char data )
{
    unsigned char commands[3] = { 0x00, 0x00, 0x00};
    unsigned char address = 0x3C;

    commands[1] = command;
    commands[2] = data;
    I2C_Write( address, commands, 3 );
}

static void ConfigureWithMoreData( unsigned char command, unsigned char start, unsigned char end )
{
    unsigned char commands[4] = { 0x00, 0x00, 0x00, 0x00};
    unsigned char address = 0x3C;

    commands[1] = command;
    commands[2] = start;
    commands[3] = end;
    I2C_Write( address, commands, 4 );
}

void Display_Clear( void )
{
    unsigned char data[2] = {0x40, 0x00};
    for( int i = 0; i < 1024; i++ )
    {
        I2C_Write( 0x3C, data, 2 );
    }
}

void Display_Init( void )
{
    /* Turn off display */
    Configure( 0xAE );
    
    /* Contrast control */
    ConfigureWithData( 0x81, 0x7F );
    
    /* Set multiplex ratio */
    ConfigureWithData( 0xA8, 63 );
    
    /* Set Display Offset */
    ConfigureWithData( 0xD3, 0x00 );
    
    /* Set Display Start Line */
    Configure( 0x40 );
    
    /* Set Segment remap */
    Configure( 0xa0 | 0x01 );
    
    /* com pin configuration */
    ConfigureWithData( 0xda, 0x02 | ( 1 << 4 ) | ( 0 << 5 ) );
    
    /* set com scan direction */
    Configure( 0xc0 | ( 1 << 3 ) );
    
    /* Set Inverse Display */
    Configure( 0xA6 );
    
    /* Set Display Clock */
    ConfigureWithData( 0xD5, ( 15 << 4 | 0x00 ) );
    
    /* Charge Pump Setting */
    ConfigureWithData( 0x8D, 0x14 );
    
    /* Entire display on */
    Configure( 0xA4 );
    
    /* Turn on display */
    Configure( 0xAF );

    /* Addressing mode */
    ConfigureWithData( 0x20, 0 );

    /* Column range */
    ConfigureWithMoreData( 0x21, 0, 127);

    /* Page Range */
    ConfigureWithMoreData( 0x22, 0, 7);

    Display_Clear();
}

