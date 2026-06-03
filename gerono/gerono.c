#include "gerono.h"
#include "qmath.h"
#include "trig.h"
#include "lut.h"

_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define NUM_WISPS (32u)

typedef struct
{
    uint8_t col;
    uint8_t page;
    uint8_t bit;
}
bit_t;

typedef struct
{
    uint8_t angle;
    pointf16_t p;
}
wisp_t;

static uint8_t display_buffer [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };

static wisp_t wisp[NUM_WISPS];

static void Set( uint8_t (* const display)[LCD_COLUMNS], bool set, const bit_t * const bit )
{    
    if( set )
    {
        display[bit->page][bit->col] |= (uint8_t)( 1U << bit->bit);
    }
    else
    {
        display[bit->page][bit->col] &= (uint8_t)(~((uint8_t)(1U << bit->bit)));
    }
}

static bit_t PointToBit(const pointf16_t * const point)
{
    uint8_t x = QMath_Int16ToUInt8(point->x, 1U);
    uint8_t y = QMath_Int16ToUInt8(point->y, 2U);

    ASSERT(x < LCD_COLUMNS);
    ASSERT(y < (LCD_ROWS * LCD_PAGES));
    
    bit_t bit =
    {
        .col   = x,
        .page  = y >> 3,
        .bit   = y & ( LCD_ROWS - 1U ),
    };

    return bit;
}

extern void Gerono_Init( void )
{
    uint8_t angle = 0;
    for(uint32_t idx = 0; idx < NUM_WISPS; idx++)
    {
        wisp[idx].p.x = 0;
        wisp[idx].p.y = 0; 
        wisp[idx].angle = angle;
        angle +=3;
    }
}

extern void Gerono_Tick( void )
{
    for(uint32_t idx = 0; idx < NUM_WISPS; idx++)
    {
        wisp_t * const w = &wisp[idx];
        bit_t prev_bit = PointToBit(&w->p);
        Set(display_buffer, false, &prev_bit);
        
        /* Gerono Lemniscate */
        int16_t x = QMath_Mul(Q15_MAX, qsin[w->angle], Q_NUM);
        int16_t y = QMath_Mul(Q15_MAX, qsin[w->angle], Q_NUM);
        y = QMath_Mul(y, qcos[w->angle], Q_NUM);

        w->p.x = x;
        w->p.y = y;
        (w->angle)+=2;
        /* Draw */
        bit_t bit = PointToBit(&w->p);
        Set(display_buffer, true, &bit);
    }
}

extern uint8_t (*Gerono_GetBuffer( void ))[LCD_COLUMNS]
{
    return display_buffer;
}
