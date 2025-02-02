#include "bird.h"

_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define NUM_BIRDS (4U)

typedef struct
{
    
}
bird_t;

static uint8_t display_buffer [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };


extern void Bird_Init( void ( *fn)( void ), uint32_t initial_seed )
{
    assert(fn != NULL);
    assert(initial_seed != 0U);
}

extern void Bird_Tick( void )
{
}

extern uint8_t (*Bird_GetBuffer( void ))[LCD_COLUMNS]
{
    return display_buffer;
}

