#ifndef _LIFE_H_
#define _LIFE_H_

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

void Life_Init( void ( *fn)( void ) );
void Life_Tick( void );
void Life_Seed( void );
unsigned char (*Life_GetBuffer( void ))[LCD_COLUMNS];

#endif /* _LIFE_H_ */
