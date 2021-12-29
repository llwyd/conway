#include "i2c.h"


/* Naff way of doing it but whatever */
#define RCC_APB1ENR1    ( *((volatile unsigned int *) 0x40021058 ) )
#define RCC_AHB2ENR     ( *((volatile unsigned int *) 0x4002104C ) )

/* Base address for GPIOA is 0x48000000 */
#define GPIOA_MODER     ( *((volatile unsigned int *) 0x48000000 ) )
#define GPIOA_OTYPER    ( *((volatile unsigned int *) 0x48000004 ) )
#define GPIOA_SPEEDER   ( *((volatile unsigned int *) 0x48000008 ) )
#define GPIOA_AFR       ( *((volatile unsigned int *) 0x48000024 ) )

/* I2C1 base = 0x40005400 */
#define I2C_CR1       ( *((volatile unsigned int *) 0x40005400 ) )
#define I2C_CR2       ( *((volatile unsigned int *) 0x40005404 ) )
#define I2C_TIMNGR    ( *((volatile unsigned int *) 0x40005410 ) )
#define I2C_ISR       ( *((volatile unsigned int *) 0x40005418 ) )
#define I2C_RXDR      ( *((volatile unsigned int *) 0x40005424 ) )

void I2C_TMP102( void )
{
    unsigned char address = 0x48;
    unsigned char data[2] = {0x00, 0x00};
    unsigned char num_bytes = 2U;

    /* 1. Set addressing mode */
    I2C_CR2 |= ( 1 << 25 ); 
    /* 2. Slave address */
    I2C_CR2 |= ( address << 1 );
    /* 3. Transfer Direction */
    I2C_CR2 |= ( 1 << 10 );
    /* 4. Number of bytes */
    I2C_CR2 |= ( num_bytes << 16 );

    /* Send start condition */
    I2C_CR2 |= ( 1 << 13 );


    /* Read data */
    unsigned char bytes_received = 0;
    unsigned char idx = 0;
    while( bytes_received < num_bytes )
    {
        if( I2C_ISR & ( 1 << 2 ) )
        {
            data[ idx++ ] = I2C_RXDR;
            bytes_received++;
        }
    }

    /* Stop condition */
    if( I2C_ISR & ( 1 << 6 ) )
    {
        I2C_CR1 |= ( 1 << 14 );
    }
}

void I2C_Init( void )
{
    /* Enable GPIOA */
    RCC_AHB2ENR |= 0x1;

    /* GPIO Config */
    /* Set Alternative Function*/
    GPIOA_MODER &= ~( 1 << 18 );
    GPIOA_MODER &= ~( 1 << 20 );

    /* Set Open Drain */
    GPIOA_OTYPER |= ( 1 << 9 );
    GPIOA_OTYPER |= ( 1 << 10 );

    /* Set to speedy */
    GPIOA_SPEEDER |= ( 0x3 << 20 );
    GPIOA_SPEEDER |= ( 0x3 << 18 );

    /* Alternate function AF4 */
    GPIOA_AFR |= ( 0x440 );

    
    /* Enable I2C1 Clock */
    RCC_APB1ENR1 |= ( 0x1 << 21 );
    
    /* configure I2C CR1 and 2 */
    //I2C_CR2 |= ( 0x1 << 25U ); /* Automatically send stop bit after n bytes */ 
    /* Rise time */
    I2C_TIMNGR |= ( 0x13 << 0 );
    I2C_TIMNGR |= ( 0xF << 8 );
    I2C_TIMNGR |= ( 0x2 << 16 );
    I2C_TIMNGR |= ( 0x4 << 20 );

    /* Enable */
    I2C_CR1 |= 0x1;

    /* PA9 = SCL
     * PA10 = SDA
     */

}
