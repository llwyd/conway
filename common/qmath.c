#include "qmath.h"

#define MAX_BITS (32)
#define MAX_Q (MAX_BITS >> 1U)

int16_t QMath_Mul(int16_t a, int16_t b, uint16_t q)
{
    assert(q < MAX_Q);

    int16_t result = (int16_t)(((int32_t)a * (int32_t)b) >> q);
    return result;
}

int16_t QMath_Add(int16_t a, int16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    
    int16_t result = a + b;
    return result;
}

int16_t QMath_Sub(int16_t a, int16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    int16_t result = a - b;
    return result;
}

int16_t QMath_Div(int16_t a, int16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    int16_t result = (int16_t)(((int32_t)a << q)/ (int32_t)b);
    return result;
}

uint16_t QMath_UMul(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);

    uint16_t result = (uint16_t)(((uint32_t)a * (uint32_t)b) >> q);
    return result;
}

uint16_t QMath_UAdd(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    
    uint16_t result = a + b;
    return result;
}

uint16_t QMath_USub(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    uint16_t result = a - b;
    return result;
}

uint16_t QMath_UDiv(uint16_t a, uint16_t b, uint16_t q)
{
    assert(q < MAX_Q);
    uint16_t result = (uint16_t)(((uint32_t)a << q)/ (uint32_t)b);
    return result;
}

