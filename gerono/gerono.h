#ifndef GERONO_H_
#define GERONO_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "life_assert.h"

#ifdef TARGET_DESKTOP
/* These parameters are for the Desktop Emulator */
#define LCD_COLUMNS ( 128 )
#define LCD_ROWS    ( 8 )
#define LCD_PAGES ( 8 )

#else
/* These parameters are for the SSD1306 Display */
#define LCD_COLUMNS ( 128 )
#define LCD_ROWS    ( 8 )
#define LCD_PAGES ( 8 )

#endif

#define LCD_FULL_ROWS ( LCD_ROWS * LCD_PAGES )

_Static_assert((LCD_COLUMNS & (LCD_COLUMNS - 1)) == 0U, "Must be power of 2!");
_Static_assert((LCD_ROWS & (LCD_ROWS - 1)) == 0U, "Must be power of 2!");
_Static_assert((LCD_PAGES & (LCD_PAGES - 1)) == 0U, "Must be power of 2!");

extern void Gerono_Init( void );
extern void Gerono_Tick( void );
extern uint8_t (*Gerono_GetBuffer( void ))[LCD_COLUMNS];

#endif /* GERONO_H_ */
