/* Conway game of life on an arm microcontroller
 * T.L. 2021
 * */

#include "i2c.h"
#include "display.h"
#include "life.h"

/* Volatile because these are memory registers */
#define RCC ( *( ( volatile unsigned int *)0x4002104C ) )
static volatile unsigned int * gpio_b   = (unsigned int *)0x48000400;
static volatile unsigned int * pin      = (unsigned int *)0x48000414;

/* Systick registers */
static volatile unsigned int * stk_ctrl     = (unsigned int *)0xE000E010;
static volatile unsigned int * stk_load     = (unsigned int *)0xE000E014;
static volatile unsigned int * stk_calib    = (unsigned int *)0xE000E01C;

/* RCC Base -> 0x40021000 */
#define RCC_CR              ( *((volatile unsigned int *) 0x40021000 ) )
#define RCC_CFGR            ( *((volatile unsigned int *) 0x40021008 ) )
#define RCC_PLLCFGR         ( *((volatile unsigned int *) 0x4002100C ) )
#define RCC_PLLSAI1         ( *((volatile unsigned int *) 0x40021010 ) )


static const unsigned int pin_num = 0x8;

__attribute__((section(".fastdata")))
void _sysTick( void )
{
    *pin ^= pin_num;
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
    unsigned char (*buffer)[LCD_PAGES] = Life_GetBuffer();
    unsigned char data[2] = { 0x40, 0x00};
    for( int i = 0; i < LCD_PAGES; i++ )
    {
        for( int j = 0; j < LCD_COLUMNS; j++ )
        {
            data[1] = buffer[j][i];
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
    *gpio_b &= 0xFFFFFF3F;
    *gpio_b |= 0x40;

    /* Configure systick */
    /* Calibration Value from datasheet */
    *stk_calib |= 0x100270F;

    /* 500ms blink */
    *stk_load |= 0x1e8480;
    /* turn on interrupt */
    *stk_ctrl |= 0x7;    
    
    asm("CPSIE IF");

    while(1)
    {
        /* Nowt */
    }

    return 0;
}
