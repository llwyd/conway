#ifndef TRIG_H_
#define TRIG_H_
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t x;
    uint8_t y;
}
point_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
}
pu16_t;


typedef struct
{
    int16_t x;
    int16_t y;
}
point16_t;

extern uint8_t TRIG_ATan2(const point_t * const a, const point_t * const b);

#endif /* TRIG_H_ */

