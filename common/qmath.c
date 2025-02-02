#include "qmath.h"

#define MAX_BITS (32)
#define MAX_Q (MAX_BITS >> 1U)

uint16_t QMath_Mul(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);

    uint16_t result = (uint16_t)(((uint32_t)a * (uint32_t)b) >> q);
    return result;
}

uint16_t QMath_Add(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    
    uint16_t result = a + b;
    return result;
}

uint16_t QMath_Sub(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    uint16_t result = a - b;
    return result;
}

uint16_t QMath_Div(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    uint16_t result = (uint16_t)(((uint32_t)a << q)/ (uint32_t)b);
    return result;
}

