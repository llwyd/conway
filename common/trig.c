#include <stdint.h>
#include <stdbool.h>
#include "life_assert.h"
#include "trig.h"
#include "qmath.h"
#include "lut.h"

#define LUT_LEN (16U)
#define PI_RADS (0x80)

#ifndef CORDIC_ITS
#define CORDIC_ITS (4U)
#endif

const uint16_t lut_atanpi2[LUT_LEN] =
{
    8192, 4836, 2555, 1297,  651,  325,  162,   81,   40,   20,   10,
    5,    2,    1,    0,    0
};

static int16_t ABS(int16_t x)
{
    int16_t result = x;
    if(x < 0)
    {
        result = QMath_Mul(x, Q_FLOAT_TO_Q(-1.f, Q_NUM), Q_NUM);
    }
    return result;
}

extern void TRIG_Translate(point_t * const p, uint8_t angle, int16_t inc)
{
    ASSERT(p!=NULL);
    int16_t x = 0U;
    int16_t y = 0U;
    
    uint8_t cos_angle = angle + DEG_90;    
    if(angle < DEG_180)
    {
        y = QMath_Mul(inc, qsin[angle], Q_NUM);
        p->y += Q_DNSCALE(y, Q_SCALE);
    }
    else
    {
        y = QMath_Mul(inc, qsin[angle - DEG_180], Q_NUM);
        p->y -= Q_DNSCALE(y, Q_SCALE);
    }

    if(cos_angle < DEG_180)
    {
        x = QMath_Mul(inc, qsin[cos_angle], Q_NUM);
        p->x += Q_DNSCALE(x, Q_SCALE);
    }
    else
    {
        x = QMath_Mul(inc, qsin[cos_angle - DEG_180], Q_NUM);
        p->x -= Q_DNSCALE(x, Q_SCALE);
    }
}

extern void TRIG_Translate16(pointf16_t * const p, uint8_t angle, int16_t inc)
{
    ASSERT(p!=NULL);

    int16_t inc_x = QMath_Mul(inc, qcos[angle], Q_NUM);
    int16_t inc_y = QMath_Mul(inc, qsin[angle], Q_NUM);

    p->x = QMath_AddSat(p->x, inc_x, Q_NUM);
    p->y = QMath_AddSat(p->y, inc_y, Q_NUM);
}

extern uint8_t TRIG_ATan2(const pointf16_t * const a, const pointf16_t * const b)
{
    /* Need div/2 to avoid over/underflow */
    int16_t diff_x = (a->x >> 1) - (b->x >> 1);
    int16_t diff_y = (a->y >> 1) - (b->y >> 1);

    int16_t abs_x = ABS(diff_x);
    int16_t abs_y = ABS(diff_y);
    
    ASSERT(abs_x >= 0);
    ASSERT(abs_y >= 0);

    uint16_t cordic_angle = 0;
    int16_t d = Q_FLOAT_TO_Q(-1.f, Q_NUM);

    ASSERT(abs_x >= 0);
    ASSERT(abs_y >= 0);

    pointf16_t c =
    {
        .x = abs_x,
        .y = abs_y,
    };
    
    for(uint32_t idx = 0; idx < CORDIC_ITS; idx++)
    {
        pointf16_t next;
        if(c.y == 0)
        {
            break;
        }

        int16_t d_y = QMath_Mul(d, c.y >> idx, Q_NUM);
        int16_t d_x = QMath_Mul(d, c.x >> idx, Q_NUM);
        
        next.x = QMath_SubSat(c.x, d_y, Q_NUM);
        next.y = QMath_AddSat(c.y, d_x, Q_NUM);
        /*
        next.x = c.x - (d * (c.y >> idx)) ;
        next.y = c.y + (d * (c.x >> idx)) ;
        */
        c = next;
       
        if( d < 0)
        {
            cordic_angle += lut_atanpi2[idx]; 
        }
        else
        {
            cordic_angle -= lut_atanpi2[idx];
        }

        d = (c.y < 0) ? Q_FLOAT_TO_Q(0.9999, Q_NUM) : Q_FLOAT_TO_Q(-1.f, Q_NUM);
    }
    
    uint8_t angle = Q_UDNSCALE(cordic_angle, 8);
    if( (diff_y < 0) && (diff_x < 0))
    {
        angle -= PI_RADS;
    }
    else if(diff_x < 0)
    {
        angle = PI_RADS - angle;
    }
    else if(diff_y < 0)
    {
        angle = -angle;
    }

    return angle;
}

extern quadrant_t TRIG_WhichQuadrant(const pointf16_t * const a, const pointf16_t * const b)
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

extern uint8_t TRIG_SAM(uint8_t a, uint8_t b)
{
    uint8_t midpoint = a;

    uint16_t a16 = (uint16_t)a;
    uint16_t b16 = (uint16_t)b;
    if(a16 < b16)
    {
        uint16_t delta_0 = b16 - a16;
        uint16_t delta_1 = ((uint16_t)(a16 + UINT8_MAX + 1) - b16);

        if(delta_0 < delta_1)
        {
            midpoint = (uint8_t)((a16 + b16) >> 1U);
        }
        else
        {
            midpoint = (uint8_t)((a16 + UINT8_MAX + 1 + b16) >> 1U);
        }
    }
    else
    {
        uint16_t delta_0 = a16 - b16;
        uint16_t delta_1 = (uint16_t)((b16 + UINT8_MAX + 1) - a16);

        if(delta_0 < delta_1)
        {
            midpoint = (uint8_t)((a16 + b16) >> 1U);
        }
        else
        {
            midpoint = (uint8_t)((a16 + UINT8_MAX + 1 + b16) >> 1U);
        }
    }
    //midpoint = (a + b) >> 1U;

    return midpoint;
}

