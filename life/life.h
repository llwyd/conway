#ifndef _LIFE_H_
#define _LIFE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef TARGET_DESKTOP
/* These parameters are for the Desktop Emulator */
#define LCD_COLUMNS ( 64 )
#define LCD_ROWS    ( 8 )
#define LCD_PAGES ( 8 )

#else
/* These parameters are for the SSD1306 Display */
#define LCD_COLUMNS ( 128 )
#define LCD_ROWS    ( 8 )
#define LCD_PAGES ( 8 )

#endif

#define LCD_FULL_ROWS ( LCD_ROWS * LCD_PAGES )

extern void Life_Init( void ( *fn)( void ), uint32_t initial_seed );
extern void Life_Tick( void );
extern void Life_Seed( void );
extern uint8_t (*Life_GetBuffer( void ))[LCD_COLUMNS];

#endif /* _LIFE_H_ */
