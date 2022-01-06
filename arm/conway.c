/* Conway game of life on an arm microcontroller
 * T.L. 2021
 * */

#include "i2c.h"
#include "display.h"
#include "life.h"

/* Volatile because these are memory registers */
#define RCC     ( *( ( volatile unsigned int *)0x4002104C ) )
#define GPIO_B  ( *( ( volatile unsigned int *)0x48000400 ) )
#define PIN     ( *( ( volatile unsigned int *)0x48000414 ) )

/* Systick registers */
#define STK_CTRL     ( *( ( volatile unsigned int *)0xE000E010 ) )
#define STK_LOAD     ( *( ( volatile unsigned int *)0xE000E014 ) )
#define STK_CALIB    ( *( ( volatile unsigned int *)0xE000E01C ) )

/* RCC Base -> 0x40021000 */
#define RCC_CR              ( *((volatile unsigned int *) 0x40021000 ) )
#define RCC_CFGR            ( *((volatile unsigned int *) 0x40021008 ) )
#define RCC_PLLCFGR         ( *((volatile unsigned int *) 0x4002100C ) )
#define RCC_PLLSAI1         ( *((volatile unsigned int *) 0x40021010 ) )

#define LED_PIN ( 0x8 )

void UpdateDisplay ( void );

__attribute__((section(".fastdata")))
void _sysTick( void )
{
     PIN ^= LED_PIN;
     Life_Tick();
     UpdateDisplay(); 
}

void ConfigureClocks( void )
{
    RCC_PLLCFGR |= ( 1 << 24 );
    
    RCC_PLLSAI1 |= ( 1 << 24 );
    
    RCC_PLLCFGR |= ( 1 << 16 );
    RCC_PLLCFGR |= ( 1 << 0 );

    RCC_CR |= ( 1 << 24 );
    while( ( RCC_CR & ( 1 << 25 ) ) != ( 1 << 25 ));
    RCC_CR |= ( 1 << 26 );
    while( ( RCC_CR & ( 1 << 27 ) ) != ( 1 << 27 ));

    RCC_CFGR |= 0x3;
    while( ( RCC_CFGR & ( 0x3 << 2 ) ) != ( 0x3 << 2 ));

}

void UpdateDisplay ( void )
{
    unsigned char (*buffer)[LCD_COLUMNS] = Life_GetBuffer();
    unsigned char data[2] = { 0x40, 0x00};
    for( int i = 0; i < LCD_PAGES; i++ )
    {
        for( int j = 0; j < LCD_COLUMNS; j++ )
        {
            data[1] = buffer[i][j];
            I2C_Write( 0x3C, data, 2 );
        }
    }
}

int main ( void )
{
    ConfigureClocks();

    I2C_Init();
    Display_Init();
    Life_Init();
    UpdateDisplay();

    /* enable port b*/
    RCC |=  0x2; 
    /* set gpio b to output */
    GPIO_B &= 0xFFFFFF3F;
    GPIO_B |= 0x40;

    /* Configure systick */
    /* Calibration Value from datasheet */
    STK_CALIB |= 0x100270F;

    /* 500ms blink */
    STK_LOAD |= 0x1e8480;
    /* turn on interrupt */
    STK_CTRL |= 0x7;    
    
    asm("CPSIE IF");

    while(1)
    {
        /* Nowt */
    }

    return 0;
}
