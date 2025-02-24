#include "bird.h"
#include "qmath.h"
#include "trig.h"

_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define NUM_BIRDS (32U)

#define SEP_RADIUS8 (0x04U)
#define COH_RADIUS8 (0x0FU)

#define SEP_ANGLE 0x06U
#define COH_ANGLE 0x01U;
#define EDGE_ANGLE 0x33U;

#define SPEED_INC (0x011d)
#define DELTA_FRACT (0x0FFF)
#define ALPHA_POINT (0x007F)
#define ALPHA (0x00FF)
#define EDGE (0x05U)

typedef struct
{
    uint8_t col;
    uint8_t page;
    uint8_t bit;
}
bit_t;

typedef enum
{
    BirdState_Idle,
    BirdState_TurningN,
    BirdState_TurningE,
    BirdState_TurningS,
    BirdState_TurningW,
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
    uint8_t angle;

    //point16_t p;
    //uint16_t a;
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

static uint32_t rng_seed = 0x13121312;

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
    ASSERT(point->x < LCD_COLUMNS);
    ASSERT(point->y < (LCD_ROWS * LCD_PAGES));
    
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

static bird_state_t NextState(const bird_t * const b)
{
    bird_state_t next_state = BirdState_Idle;

    uint8_t r = LCD_COLUMNS - EDGE;
    uint8_t l = 0U + EDGE;
    uint8_t u = 0U + EDGE;
    uint8_t d = LCD_FULL_ROWS - EDGE;

    const point_t * const p = &b->pos;

    if( (p->x < l) )
    {
        next_state = BirdState_TurningW;
    }
    else if( (p->x > r) )
    {
        next_state = BirdState_TurningE;
    }
    else if( (p->y < u) )
    {
        next_state = BirdState_TurningN;
    }
    else if( (p->y > d) )
    {
        next_state = BirdState_TurningS;
    }
    else
    {
        next_state = BirdState_Idle;
    }


    return next_state;
}

static void ScreenWrap(bird_t * const b)
{
        b->pos.x = b->pos.x & (LCD_COLUMNS - 1U);
        b->pos.y = b->pos.y & (LCD_FULL_ROWS - 1U);
}

extern void Bird_Init( void ( *fn)( void ), uint32_t initial_seed )
{
    ASSERT(fn != NULL);
    ASSERT(initial_seed != 0U);
    
    uint32_t rng = 0x13121312;

    for( uint32_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        rng = xorshift32(rng);
        uint8_t x = (uint8_t)(rng >> 24U);
        uint8_t y = (uint8_t)(rng >> 16U);
        bird[idx].angle = (uint16_t)(rng >> 8U);
        //bird[idx].angle = 24U;
        bird[idx].state = BirdState_Idle;

        bird[idx].pos.x = x & ( (LCD_COLUMNS) - 1U);
        bird[idx].pos.y = y & ( (LCD_FULL_ROWS) - 1U);
        
        ASSERT(bird[idx].pos.x < LCD_COLUMNS);
        ASSERT(bird[idx].pos.y < LCD_FULL_ROWS);
    }

    update_fn = fn;
}

static bool IsPointInSquare8(const point_t * const b, const point_t * const c, uint8_t square_size)
{
    uint8_t ss_2 = square_size >> 0U;
    ASSERT(ss_2 > 0);
    
    bool result = false;
    uint8_t r = c->x + ss_2;
    uint8_t l = c->x - ss_2;
    uint8_t u = c->y - ss_2;
    uint8_t d = c->y + ss_2;

    if(u >= LCD_FULL_ROWS)
    {
        u = 0;
    }
    if(l >= LCD_COLUMNS)
    {
        l = 0;
    }

    if((b->x > l) && (b->x < r))
    {
        if((b->y > u) && (b->y < d))
        {
            result = true;
        }
    }
    return result;
}

static void CollectNearbyBirds8(bird_t * const current_bird, nearby_t * const near_birds, uint16_t square_size)
{
    ASSERT(near_birds != NULL);
    ASSERT(square_size > 1U);

    const point_t * const c = &current_bird->pos;
    near_birds->num = 0U;

    for(uint32_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        if(current_bird != &bird[idx])
        {
            const point_t * const b = &bird[idx].pos;
            
            if(IsPointInSquare8(b, c, square_size))
            {
                if(bird[idx].state == BirdState_Idle)
                {
                    near_birds->bird[near_birds->num] = idx;
                    near_birds->num++;
                }
            }
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
        point16_t pos = 
        { 
            .x = Q_UPSCALE(bird[nearby_idx].pos.x, Q_SCALE),
            .y = Q_UPSCALE(bird[nearby_idx].pos.y, Q_SCALE),
        };
        
        int16_t diff_x = QMath_Sub(pos.x, prev.x, Q_NUM);
        int16_t diff_y = QMath_Sub(pos.y, prev.y, Q_NUM);

        result.x = pos.x - QMath_Mul(ALPHA_POINT, diff_x, Q_NUM);
        result.y = pos.y - QMath_Mul(ALPHA_POINT, diff_y, Q_NUM);
    }

    return result;
}

extern uint16_t AverageAngle(const nearby_t * const nearby)
{
    uint16_t result = 0;
    uint16_t y = 0;

    for(uint32_t idx = 0; idx < nearby->num; idx++)
    {
        uint16_t prev_y = y;
        uint8_t nearby_idx = nearby->bird[idx];
        uint16_t angle = Q_UUPSCALE(bird[nearby_idx].angle,Q_SCALE);
        uint16_t diff = QMath_USub(angle, prev_y, Q_NUM);
        y = angle - QMath_UMul(ALPHA, diff, Q_NUM);
    }

    result = y;
    return result;
}


extern void Idle( bird_t * const b)
{
    quadrant_t quad = Quad_0;
    /* Collect nearby birds */
    CollectNearbyBirds8(b, &nearby_sep, SEP_RADIUS8);
    CollectNearbyBirds8(b, &nearby_else, COH_RADIUS8);

    if(nearby_sep.num > 0U)
    {
        /* Handle separation */
        /* Determine angle from quadrant */
        const point16_t avg_pos = AveragePoint(&nearby_sep); 
        const uint8_t avg_x = Q_DNSCALE(avg_pos.x, Q_SCALE);
        const uint8_t avg_y = Q_DNSCALE(avg_pos.y, Q_SCALE);
        const point_t avg = {.x=avg_x, .y=avg_y};
        quadrant_t q = WhichQuadrant(&b->pos, &avg);
        uint8_t a = TRIG_ATan2(&b->pos, &avg);

        switch(q)
        {
            case Quad_0:
                if(a > 160)
                {
                    b->angle -= SEP_ANGLE;
                }
                else
                {
                    b->angle += SEP_ANGLE;
                }
                break;
            case Quad_3:
                if(a < 96)
                {
                    b->angle += SEP_ANGLE;
                }
                else
                {
                    b->angle -= SEP_ANGLE;
                }
                break;
            case Quad_1:
                if(a < 96 )
                {
                    b->angle -= SEP_ANGLE;
                }
                else
                {
                    b->angle += SEP_ANGLE;
                }
                break;
            case Quad_2:
                if(a > 160 )
                {
                    b->angle += SEP_ANGLE;
                }
                else
                {
                    b->angle -= SEP_ANGLE;
                }
                break;
        }
        //TRIG_Translate(&b->pos, b->angle, SPEED_INC);
        ScreenWrap(b);
    }
    else if(nearby_else.num > 0U)
    {
        /* Handle Alignment + Cohesion */
        /* Determine angle from quadrant */
        const point16_t avg_pos = AveragePoint(&nearby_else);
        const uint8_t avg_x = Q_DNSCALE(avg_pos.x, Q_SCALE);
        const uint8_t avg_y = Q_DNSCALE(avg_pos.y, Q_SCALE);
        const point_t avg = {.x=avg_x, .y=avg_y};
        quadrant_t q = WhichQuadrant(&b->pos, &avg);
        uint8_t a = TRIG_ATan2(&b->pos, &avg);
        quad = q;
        switch(q)
        {
            case Quad_0:
                if(a > 160)
                {
                    b->angle += COH_ANGLE;
                }
                else
                {
                    b->angle -= COH_ANGLE;
                }
                break;
            case Quad_3:
                if(a < 96)
                {
                    b->angle -= COH_ANGLE;
                }
                else
                {
                    b->angle += COH_ANGLE;
                }
                break;
            case Quad_1:
                if(a < 96 )
                {
                    b->angle += COH_ANGLE;
                }
                else
                {
                    b->angle -= COH_ANGLE;
                }
                break;
            case Quad_2:
                if(a > 160 )
                {
                    b->angle -= COH_ANGLE;
                }
                else
                {
                    b->angle += COH_ANGLE;
                }
                break;
        }

    }

    /* Update bird
     * -> Move
     * -> Update state machine
     * -> Screen wrap */
    rng_seed = xorshift32(rng_seed);
    uint32_t speed = (rng_seed & 0x00FF) + 0x00FF;
    TRIG_Translate(&b->pos, b->angle, speed);
    ScreenWrap(b);


    if(nearby_sep.num > 0U)
    {
    }
    else if(nearby_else.num > 0U)
    {
        uint16_t angle = Q_UUPSCALE(b->angle, Q_SCALE);
        uint16_t near_angle = AverageAngle(&nearby_else);
        uint16_t delta = (angle + near_angle) >> 1U;
        switch(quad)
        {
            case Quad_0:
            case Quad_3:
                b->angle = Q_UDNSCALE(delta, Q_SCALE);
                break;
            case Quad_1:
            case Quad_2:
                b->angle = Q_UDNSCALE(delta, Q_SCALE);
                break;
        }
    }
}

static void Turning(bird_t * const b)
{
    switch(b->state)
    {
        case BirdState_TurningS:
        {
            if( (b->angle > DEG_90) && (b->angle < DEG_270))
            {
                b->angle += EDGE_ANGLE;
            }
            else
            {
                b->angle -= EDGE_ANGLE;
            }
            break;
        }
        case BirdState_TurningN:
        {
            if( (b->angle > DEG_90) && (b->angle < DEG_270))
            {
                b->angle -= EDGE_ANGLE;
            }
            else
            {
                b->angle += EDGE_ANGLE;
            }
            break;
        }
        case BirdState_TurningE:
        {
            if( (b->angle > DEG_0) && (b->angle < DEG_180))
            {
                b->angle += EDGE_ANGLE;
            }
            else
            {
                b->angle -= EDGE_ANGLE;
            }
            break;
        }
        case BirdState_TurningW:
        {
            if( (b->angle > DEG_0) && (b->angle < DEG_180))
            {
                b->angle -= EDGE_ANGLE;
            }
            else
            {
                b->angle += EDGE_ANGLE;
            }
            break;
        }
        default:
            ASSERT(false);
            break;
    }
    TRIG_Translate(&b->pos, b->angle, SPEED_INC);
    ScreenWrap(b);
}

extern void Bird_Tick( void )
{
    for(uint8_t idx = 0; idx < NUM_BIRDS; idx++)
    {

        bird_t * const b = &bird[idx];
        bit_t prev_bit = PointToBit(&b->pos);
        Set(display_buffer, false, &prev_bit);
        
        switch(b->state)
        {
            case BirdState_Idle:
                Idle(b);
                break;
            case BirdState_TurningN:
            case BirdState_TurningE:
            case BirdState_TurningS:
            case BirdState_TurningW:
                Turning(b);
                break;
            default:
                ASSERT(false);
                break; 
        }
        
        b->state = NextState(b);

        /* Draw */
        bit_t bit = PointToBit(&b->pos);
        Set(display_buffer, true, &bit);
    }
    update_fn();
}

extern uint8_t (*Bird_GetBuffer( void ))[LCD_COLUMNS]
{
    return display_buffer;
}

