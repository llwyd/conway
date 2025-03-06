#include "bird.h"
#include "qmath.h"
#include "trig.h"

_Static_assert(sizeof(uint8_t) == 1U, "invalid u8 size");
_Static_assert(LCD_PAGES <= UINT8_MAX, "invalid num of pages");
_Static_assert(LCD_COLUMNS <= UINT8_MAX, "invalid cols");
_Static_assert(LCD_ROWS == 8U, "must be u8");

#define NUM_BIRDS (72U)

#define SEP_RADIUS8 (0x00A0)
#define COH_RADIUS8 (0x2000)

#define SEP_ANGLE 0x0FU
#define COH_ANGLE 0x01U;
#define EDGE_ANGLE 0x10U;

#define SPEED_INC (0x05FF)
#define ALPHA_POINT (0x03FF)
#define ALPHA (0x007F)
#define EDGE (0x0120)

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

typedef struct
{
    uint8_t angle;
    pointf16_t p;
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

static bit_t PointToBit(const pointf16_t * const point);
static void Set( uint8_t (* const display)[LCD_COLUMNS], bool set, const bit_t * const bit );

static uint32_t xorshift32( uint32_t x )
{
    x ^= x << 13U;
    x ^= x >> 17U;
    x ^= x << 5U;

    return x;
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

    int16_t r = QMath_SubSat(Q15_MAX, EDGE, Q_NUM);
    int16_t l = QMath_AddSat(Q15_MIN, EDGE, Q_NUM);
    int16_t u = QMath_AddSat(Q15_MIN, EDGE, Q_NUM);
    int16_t d = QMath_SubSat(Q15_MAX, EDGE, Q_NUM);

    const pointf16_t * const p = &b->p;

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
    (void)b;
    /*
        b->pos.x = b->pos.x & (LCD_COLUMNS - 1U);
        b->pos.y = b->pos.y & (LCD_FULL_ROWS - 1U);
    */
}

extern void Bird_Init( void ( *fn)( void ), uint32_t initial_seed )
{
    ASSERT(fn != NULL);
    ASSERT(initial_seed != 0U);
    
    uint32_t rng = 0x13121312;

    for( uint32_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        rng = xorshift32(rng);
        int16_t x = (int16_t)(rng >> 16U);
        rng = xorshift32(rng);
        int16_t y = (int16_t)(rng >> 16U);
        bird[idx].angle = (uint16_t)(rng >> 8U);
        //bird[idx].angle = 24U;
        bird[idx].state = BirdState_Idle;

        bird[idx].p.x = x;
        bird[idx].p.y = y;
    }

    update_fn = fn;
}

static bool IsPointInSquare8(const pointf16_t * const b, const pointf16_t * const c, int16_t square_size)
{
    int16_t ss_2 = square_size >> 0U;
    ASSERT(ss_2 > 0);
    
    bool result = false;
    int16_t r = QMath_AddSat(c->x, ss_2, Q_NUM);
    int16_t l = QMath_SubSat(c->x, ss_2, Q_NUM);
    int16_t u = QMath_SubSat(c->y, ss_2, Q_NUM);
    int16_t d = QMath_AddSat(c->y, ss_2, Q_NUM);

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

static void CollectNearbyBirds8(bird_t * const current_bird, nearby_t * const near_birds, int16_t square_size)
{
    ASSERT(near_birds != NULL);
    ASSERT(square_size > 1);

    const pointf16_t * const c = &current_bird->p;
    near_birds->num = 0U;

    for(uint32_t idx = 0; idx < NUM_BIRDS; idx++)
    {
        if(current_bird != &bird[idx])
        {
            const pointf16_t * const b = &bird[idx].p;
            
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

extern pointf16_t AveragePoint(const nearby_t * const nearby)
{
    pointf16_t result ={.x = 0, .y = 0};
    
    for(uint32_t idx = 0; idx < nearby->num; idx++)
    {
        pointf16_t prev = 
        {
            .x = result.x,
            .y = result.y,
        };

        uint8_t nearby_idx = nearby->bird[idx];
        pointf16_t * pos = &bird[nearby_idx].p;
        
        int16_t diff_x = QMath_Sub(pos->x, prev.x, Q_NUM);
        int16_t diff_y = QMath_Sub(pos->y, prev.y, Q_NUM);

        diff_x = QMath_Mul(ALPHA_POINT, diff_x, Q_NUM);
        diff_x = QMath_Mul(ALPHA_POINT, diff_y, Q_NUM);


        result.x = QMath_SubSat(pos->x, diff_x, Q_NUM);
        result.y = QMath_SubSat(pos->y, diff_y, Q_NUM);
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
        const pointf16_t avg = AveragePoint(&nearby_sep); 
        quadrant_t q = TRIG_WhichQuadrant(&b->p, &avg);
        uint8_t a = TRIG_ATan2(&b->p, &avg);
        switch(q)
        {
            case Quad_0:
                if((a > DEG_225) || (a < DEG_45))
                {
                    b->angle -= SEP_ANGLE;
                }
                else
                {
                    b->angle += SEP_ANGLE;
                }
                break;
            case Quad_3:
                if((a < DEG_135) || (a > DEG_315))
                {
                    b->angle += SEP_ANGLE;
                }
                else
                {
                    b->angle -= SEP_ANGLE;
                }
                break;
            case Quad_1:
                if( (a < DEG_135) || (a > DEG_315) )
                {
                    b->angle -= SEP_ANGLE;
                }
                else
                {
                    b->angle += SEP_ANGLE;
                }
                break;
            case Quad_2:
                if((a > DEG_225) || (a < DEG_45) )
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
    if(nearby_else.num > 0U)
    {
        /* Handle Alignment + Cohesion */
        /* Determine angle from quadrant */
        const pointf16_t avg = AveragePoint(&nearby_else);
        quadrant_t q = TRIG_WhichQuadrant(&b->p, &avg);
        uint8_t a = TRIG_ATan2(&b->p, &avg);
        quad = q;
        switch(q)
        {
            case Quad_0:
                if((a > DEG_225) || (a < DEG_45))
                {
                    b->angle += COH_ANGLE;
                }
                else
                {
                    b->angle -= COH_ANGLE;
                }
                break;
            case Quad_3:
                if((a < DEG_135) || (a > DEG_315))
                {
                    b->angle -= COH_ANGLE;
                }
                else
                {
                    b->angle += COH_ANGLE;
                }
                break;
            case Quad_1:
                if( (a < DEG_135) || (a > DEG_315) )
                {
                    b->angle += COH_ANGLE;
                }
                else
                {
                    b->angle -= COH_ANGLE;
                }
                break;
            case Quad_2:
                if((a > DEG_225) || (a < DEG_45) )
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
    /* TODO - Fix */
    rng_seed = xorshift32(rng_seed);
    uint32_t speed = (rng_seed & 0x07FF) + 0x03FF;
    TRIG_Translate16(&b->p, b->angle, speed);
    ScreenWrap(b);

    if(nearby_else.num > 0U)
    {
        uint16_t near_angle = AverageAngle(&nearby_else);
        uint8_t near_angle8 = Q_UDNSCALE(near_angle, Q_SCALE);
        uint8_t delta = TRIG_SAM(b->angle, near_angle8);
        switch(quad)
        {
            case Quad_0:
            case Quad_3:
                b->angle = (uint8_t) delta;
                break;
            case Quad_1:
            case Quad_2:
                b->angle = (uint8_t)delta;
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
    TRIG_Translate16(&b->p, b->angle, SPEED_INC);
    ScreenWrap(b);
}

extern void Bird_Tick( void )
{
    for(uint8_t idx = 0; idx < NUM_BIRDS; idx++)
    {

        bird_t * const b = &bird[idx];
        bit_t prev_bit = PointToBit(&b->p);
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
        bit_t bit = PointToBit(&b->p);
        Set(display_buffer, true, &bit);
    }
    update_fn();
}

extern uint8_t (*Bird_GetBuffer( void ))[LCD_COLUMNS]
{
    return display_buffer;
}

