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

extern void Heart_Init( void )
{
    uint8_t angle = 0;
    for(uint32_t idx = 0; idx < NUM_WISPS; idx++)
    {
        wisp[idx].p.x = 0;
        wisp[idx].p.y = 0; 
        wisp[idx].angle = angle;
        angle +=5;
    }
}

extern void Heart_Tick( void )
{
    for(uint32_t idx = 0; idx < NUM_WISPS; idx++)
    {
        wisp_t * const w = &wisp[idx];
        bit_t prev_bit = PointToBit(&w->p);
        Set(display_buffer, false, &prev_bit);

        /* Heart */

        /* Is angle over 180 deg? */
        uint8_t sign_bit = w->angle >> 7;

        /* only use 0 -> pi */
        uint8_t angle = w->angle & 0x7f;

        /* sin^3(x) */
        int16_t x = QMath_Mul(qsin[angle], qsin[angle], Q_NUM);
        x = QMath_Mul(x, qsin[angle], Q_NUM);

        /* _ * 0.5 */
        x = QMath_Mul(0x3FFF, x, Q_NUM);
       
        x = QMath_Mul(0x7FFF + sign_bit, x, Q_NUM);
       

        uint8_t y_angle = angle ^ ((UINT8_MAX >> 1) * (sign_bit ^ 0x1));

        int16_t a = QMath_Mul(0x6800, qcos[y_angle * 1], Q_NUM);
        int16_t b = QMath_Mul(0x2800, qcos[y_angle * 2], Q_NUM);
        int16_t c = QMath_Mul(0x1000, qcos[y_angle * 3], Q_NUM);
        int16_t d = QMath_Mul(0x0800, qcos[y_angle * 4], Q_NUM);


        int16_t y = QMath_SubSat(a, b, Q_NUM);
        y = QMath_SubSat(y, c, Q_NUM);
        y = QMath_SubSat(y, d, Q_NUM);

        /* Invert y axis */
        y = QMath_Mul(y, 0x8001, Q_NUM);

        w->p.x = x;
        w->p.y = y;
        (w->angle)+=2;

        //w->angle %= UINT8_MAX >> 1;
        /* Draw */
        bit_t bit = PointToBit(&w->p);
        Set(display_buffer, true, &bit);
    }
}

extern uint8_t (*Heart_GetBuffer( void ))[LCD_COLUMNS]
{
    return display_buffer;
}
