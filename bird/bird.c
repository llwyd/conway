#include "bird.h"
#include "qmath.h"
#include "lut.h"

_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define NUM_BIRDS (8U)
#define Q_NUM (15U)
#define Q_SCALE (Q_NUM - 8U)

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

/* Used to collect info of nearby birds based on index */
typedef struct
{
    uint8_t bird[NUM_BIRDS];
    uint8_t num;
}
nearby_t;

static uint8_t display_buffer [ LCD_PAGES ] [ LCD_COLUMNS ] = { 0x00 };
static bird_t bird[NUM_BIRDS];

/* Separation and cohesion have different radii */
static nearby_t nearby_sep;
static nearby_t nearby_else;


void ( *update_fn )( void );

static bit_t PointToBit(const point_t * const point);
static void Set( uint8_t (* const display)[LCD_COLUMNS], bool set, const bit_t * const bit );

static uint32_t xorshift32( uint32_t x )
{
    x ^= x << 13U;
    x ^= x >> 17U;
    x ^= x << 5U;

    return x;
}

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

static point_t Move(bird_t * const bird)
{
    point_t prev = bird->pos;

    /* x = inc + cos(theta) */
    /* y = inc + sin(theta) */
    int16_t x = Q_UPSCALE(bird->pos.x, Q_SCALE);
    int16_t y = Q_UPSCALE(bird->pos.y, Q_SCALE);

    const uint8_t angle = bird->angle;
    
    x = x + QMath_Mul(500, qcos[angle], Q_NUM);
    y = y + QMath_Mul(500, qsin[angle], Q_NUM);

    bird->pos.x = Q_DNSCALE(x, Q_SCALE);
    bird->pos.y = Q_DNSCALE(y, Q_SCALE);

    return prev;
}

static void ScreenWrap(bird_t * const bird)
{
        bird->pos.x = bird->pos.x & (LCD_COLUMNS - 1U);
        bird->pos.y = bird->pos.y & (LCD_FULL_ROWS - 1U);
}

extern void Bird_Init( void ( *fn)( void ), uint32_t initial_seed )
{
    assert(fn != NULL);
    assert(initial_seed != 0U);
    
    uint32_t rng = 0x13121312;

    for( uint32_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        rng = xorshift32(rng);
        uint8_t x = (uint8_t)(rng >> 24U);
        uint8_t y = (uint8_t)(rng >> 16U);
        bird[idx].angle = (uint8_t)(rng >> 8U);
        bird[idx].state = BirdState_Idle;

        bird[idx].pos.x = x & ( (LCD_COLUMNS >> 1U) - 1U);
        bird[idx].pos.y = y & ( (LCD_FULL_ROWS >> 1U) - 1U);

        assert(bird[idx].pos.x < LCD_COLUMNS);
        assert(bird[idx].pos.y < LCD_FULL_ROWS);
    }

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

        /* Update bird
         * -> Move
         * -> Update state machine
         * -> Screen wrap */
        
        point_t prev = Move(&bird[idx]);
        ScreenWrap(&bird[idx]);
        /* Draw */
        bit_t prev_bit = PointToBit(&prev);
        Set(display_buffer, false, &prev_bit);
        bit_t bit = PointToBit(&bird[idx].pos);
        Set(display_buffer, true, &bit);
    }
    update_fn();
}

extern uint8_t (*Bird_GetBuffer( void ))[LCD_COLUMNS]
{
    return display_buffer;
}

