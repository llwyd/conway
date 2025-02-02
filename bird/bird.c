#include "bird.h"

_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define NUM_BIRDS (1U)

typedef struct
{
    uint8_t col;
    uint8_t page;
    uint8_t bit;
}
bit_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
}
point_t;

typedef enum
{
    BirdState_Idle,
    BirdState_TurningX,
    BirdState_TurningY,
}
bird_state_t;

typedef struct
{
    point_t pos;
    uint8_t speed;
    uint16_t angle;
    bird_state_t state;
}
bird_t;

static uint8_t display_buffer [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };

static bird_t bird[NUM_BIRDS];

void ( *update_fn )( void );

static bit_t PointToBit(const point_t * const point);
static void Set( uint8_t (* const display)[LCD_COLUMNS], bool set, const bit_t * const bit );

static bit_t PointToBit(const point_t * const point)
{
    assert(point->x < LCD_COLUMNS);
    assert(point->y < (LCD_ROWS * LCD_PAGES));
    
    bit_t bit =
    {
        .col   = point->x,
        .page  = point->y >> 3,
        .bit   = point->y & ( LCD_ROWS - 1U ),
    };

    return bit;
}

static void Set( uint8_t (* const display)[LCD_COLUMNS], bool set, const bit_t * const bit )
{    
    if( set )
    {
        display[bit->page][bit->col] |= ( 1U << bit->bit);
    }
    else
    {
        display[bit->page][bit->col] &= ~(1U << bit->bit);
    }
}

extern void Bird_Init( void ( *fn)( void ), uint32_t initial_seed )
{
    assert(fn != NULL);
    assert(initial_seed != 0U);

    bird[0].pos.x = 64;
    bird[0].pos.y = 32;
    bird[0].angle = 0U;
    bird[0].state = BirdState_Idle;
    update_fn = fn;
}

extern void Bird_Tick( void )
{
    for(uint32_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        /* Collect nearby birds */

        /* Handle separation */

        /* Handle Alignment */

        /* Handle cohesion */

        /* Draw */
        bit_t bit = PointToBit(&bird[idx].pos);
        Set(display_buffer, true, &bit);
    }
    update_fn();
}

extern uint8_t (*Bird_GetBuffer( void ))[LCD_COLUMNS]
{
    return display_buffer;
}

