#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "trig.h"
#include "qmath.h"
#include "lut.h"

#define LUT_LEN (16U)
#define PI_RADS (0x80)

#ifndef CORDIC_ITS
#define CORDIC_ITS (4U)
#endif

#define SPEED (0x100)
const uint16_t lut_atanpi2[LUT_LEN] =
{
    8192, 4836, 2555, 1297,  651,  325,  162,   81,   40,   20,   10,
    5,    2,    1,    0,    0
};

static int16_t ABS(int16_t x)
{
    const int16_t mask = x >> 15U;
    x ^= mask;
    const int16_t result = x - mask;
    return result;
}

extern void TRIG_Translate(point_t * const p, uint8_t angle)
{
    assert(p!=NULL);
    int16_t x = 0U;
    int16_t y = 0U;
    
    uint8_t cos_angle = angle + DEG_90;    
    if(angle < DEG_180)
    {
        y = QMath_Mul(0x100, qsin[angle], Q_NUM);
        p->y += Q_DNSCALE(y, Q_SCALE);
    }
    else
    {
        y = QMath_Mul(0x100, qsin[angle - DEG_180], Q_NUM);
        p->y -= Q_DNSCALE(y, Q_SCALE);
    }

    if(cos_angle < DEG_180)
    {
        x = QMath_Mul(0x100, qsin[cos_angle], Q_NUM);
        p->x += Q_DNSCALE(x, Q_SCALE);
    }
    else
    {
        x = QMath_Mul(0x100, qsin[cos_angle - DEG_180], Q_NUM);
        p->x -= Q_DNSCALE(x, Q_SCALE);
    }
}

extern uint8_t TRIG_ATan2(const point_t * const a, const point_t * const b)
{
    int16_t diff_x = (int16_t)a->x - (int16_t)b->x;
    int16_t diff_y = (int16_t)a->y - (int16_t)b->y;

    int16_t abs_x = ABS(diff_x);
    int16_t abs_y = ABS(diff_y);
    uint16_t cordic_angle = 0;
    int16_t d = -1;

    assert(abs_x >= 0);
    assert(abs_y >= 0);

    point_t c =
    {
        .x = (abs_x << 0),
        .y = (abs_y << 0),
    };
    
    for(uint32_t idx = 0; idx < CORDIC_ITS; idx++)
    {
        point_t next;
        if(c.y == 0)
        {
            break;
        }
        next.x = c.x - (d * (c.y >> idx)) ;
        next.y = c.y + (d * (c.x >> idx)) ;

        c = next;
        
        cordic_angle += (d < 0) ? lut_atanpi2[idx] : -lut_atanpi2[idx];
        d = (c.y < 0) ? 1 : -1;
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
