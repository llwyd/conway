#include "bird.h"
#include "qmath.h"
#include "lut.h"

_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define NUM_BIRDS (16U)
#define Q_NUM (15U)
#define Q_SCALE (Q_NUM - 8U)

#define SEP_RADIUS (0x0100)
#define COH_RADIUS (0x0F00)

#define SEP_ANGLE (0x200)
#define COH_ANGLE (0x050)

#define SPEED_INC (250U)
#define ALPHA (0x0800)

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


typedef struct
{
    int16_t x;
    int16_t y;
}
point16_t;

typedef enum
{
    BirdState_Idle,
    BirdState_TurningX,
    BirdState_TurningY,
}
bird_state_t;

typedef enum
{
    Quad_0,
    Quad_1,
    Quad_2,
    Quad_3,
}
quadrant_t;

typedef struct
{
    point_t pos;
    uint8_t speed;
    int16_t angle;
    point16_t p;
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
    int16_t x = bird->p.x;
    int16_t y = bird->p.y;

    const uint8_t angle = Q_DNSCALE(bird->angle, Q_SCALE);
    
    bird->p.x = x + QMath_Mul(SPEED_INC, qcos[angle], Q_NUM);
    bird->p.y = y + QMath_Mul(SPEED_INC, qsin[angle], Q_NUM);

    bird->pos.x = Q_DNSCALE(bird->p.x, Q_SCALE);
    bird->pos.y = Q_DNSCALE(bird->p.y, Q_SCALE);

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
        bird[idx].angle = (uint16_t)(rng >> 8U);
        bird[idx].state = BirdState_Idle;

        bird[idx].pos.x = x & ( (LCD_COLUMNS >> 1U) - 1U);
        bird[idx].pos.y = y & ( (LCD_FULL_ROWS >> 1U) - 1U);

        bird[idx].p.x = Q_UPSCALE(x, Q_SCALE);
        bird[idx].p.y = Q_UPSCALE(y, Q_SCALE);
        
        assert(bird[idx].pos.x < LCD_COLUMNS);
        assert(bird[idx].pos.y < LCD_FULL_ROWS);
    }

    update_fn = fn;
}

static bool IsPointInSquare(const point16_t * const b, const point16_t * const c, int16_t square_size)
{
    int16_t ss_2 = square_size >> 1U;
    bool result = false;

    assert(ss_2 > 0);
    if((b->x < (c->x + ss_2)) && (b->x > (c->x - ss_2)))
    {
        if((b->y < (c->y + ss_2)) && (b->y > (c->y - ss_2)))
        {
            result = true;
        }
    }
    return result;
}

static void CollectNearbyBirds(uint8_t current_idx, nearby_t * const near_birds, int16_t square_size)
{
    assert(near_birds != NULL);
    assert(square_size > 1U);

    const point16_t * const c = &bird[current_idx].p;
    near_birds->num = 0U;

    for(uint32_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        if(idx == current_idx)
        {
            continue;
        }

        const point16_t * const b = &bird[idx].p;
        
        if(IsPointInSquare(b, c, square_size));
        {
            near_birds->bird[near_birds->num] = idx;
            near_birds->num++;
        }
    }

}

static quadrant_t WhichQuadrant(const point_t * const a, const point_t * const b)
{
    quadrant_t quadrant = Quad_0;
    if(a->x < b->x)
    {
       if(a->y < b->y)
       {
            quadrant = Quad_0;
       }
       else
       {
            quadrant = Quad_3;
       }
    }
    else
    {
       if(a->y < b->y)
       {
            quadrant = Quad_1;
       }
       else
       {
            quadrant = Quad_2;
       }
    }

    return quadrant;
}

extern point16_t AveragePoint(const nearby_t * const nearby)
{
    point16_t result ={.x = 0, .y = 0};
    
    for(uint32_t idx = 0; idx < nearby->num; idx++)
    {
        point16_t prev = 
        {
            .x = result.x,
            .y = result.y,
        };

        uint8_t nearby_idx = nearby->bird[idx];
        point16_t pos = bird[nearby_idx].p;
        int16_t diff_x = QMath_Sub(pos.x, prev.x, Q_NUM);
        int16_t diff_y = QMath_Sub(pos.y, prev.y, Q_NUM);

        result.x = pos.x - QMath_Mul(ALPHA, diff_x, Q_NUM);
        result.y = pos.y - QMath_Mul(ALPHA, diff_y, Q_NUM);
    }

    return result;
}

extern int16_t AverageAngle(const nearby_t * const nearby)
{
    int16_t result = 0;
    int16_t y = 0;

    for(uint32_t idx = 0; idx < nearby->num; idx++)
    {
        int16_t prev_y = y;
        uint8_t nearby_idx = nearby->bird[idx];
        int16_t angle = bird[nearby_idx].angle;
        int16_t diff = QMath_Sub(angle, prev_y, Q_NUM);
        y = angle - QMath_Mul(ALPHA, diff, Q_NUM);
    }

    result = y;
    return result;
}



extern void Bird_Tick( void )
{
    for(uint8_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        /* Collect nearby birds */
        CollectNearbyBirds(idx, &nearby_sep, SEP_RADIUS);
        CollectNearbyBirds(idx, &nearby_else, COH_RADIUS);

        if(nearby_sep.num > 0U)
        {
            /* Handle separation */
            /* Determine angle from quadrant */
            const point16_t avg_pos = AveragePoint(&nearby_sep); 
            const uint8_t avg_x = Q_DNSCALE(avg_pos.x, Q_SCALE);
            const uint8_t avg_y = Q_DNSCALE(avg_pos.y, Q_SCALE);
            const point_t avg = {.x=avg_x, .y=avg_y};
            quadrant_t q = WhichQuadrant(&bird[idx].pos, &avg);

            switch(q)
            {
                case Quad_0:
                case Quad_3:
                    bird[idx].angle+= SEP_ANGLE;
                    break;
                case Quad_1:
                case Quad_2:
                    bird[idx].angle-= SEP_ANGLE;
                    break;
            }
        }
        if(nearby_else.num > 0U)
        {
            /* Handle Alignment + Cohesion */
            /* Determine angle from quadrant */
            const point16_t avg_pos = AveragePoint(&nearby_else);
            const uint8_t avg_x = Q_DNSCALE(avg_pos.x, Q_SCALE);
            const uint8_t avg_y = Q_DNSCALE(avg_pos.y, Q_SCALE);
            const point_t avg = {.x=avg_x, .y=avg_y};
            quadrant_t q = WhichQuadrant(&bird[idx].pos, &avg);
            switch(q)
            {
                case Quad_0:
                case Quad_3:
                    bird[idx].angle-= COH_ANGLE;
                    break;
                case Quad_1:
                case Quad_2:
                    bird[idx].angle+= COH_ANGLE;
                    break;
            }

        }

        /* Update bird
         * -> Move
         * -> Update state machine
         * -> Screen wrap */
        
        point_t prev = Move(&bird[idx]);
        ScreenWrap(&bird[idx]);

        if(nearby_else.num > 0U)
        {
            int16_t near_angle = AverageAngle(&nearby_else);
            uint16_t new_angle = (bird[idx].angle - near_angle);
            bird[idx].angle += QMath_Mul(0x200, new_angle, Q_NUM);
        }
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

