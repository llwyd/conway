#include "qmath.h"
#include "life_assert.h"

#define MAX_BITS (32)
#define MAX_Q (MAX_BITS >> 1U)

uint8_t QMath_Int16ToUInt8(int16_t x, uint8_t scale)
{
    ASSERT(scale > 0);
    uint8_t result = 0;
    int16_t half = 0x8000 >> scale;

    int16_t x_scaled = x >> scale;
    x_scaled = QMath_Add(x_scaled, half, Q_NUM15);
    
    /* Scale down to int8 */
    int8_t x16 = (int8_t)QNUM_DNSCALE(x_scaled, Q_NUM15, Q_NUM7);
    result = (uint8_t)x16;

    return result;
}

int16_t QMath_Mul(int16_t a, int16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);

    int16_t result = (int16_t)(((int32_t)a * (int32_t)b) >> q);
    return result;
}

int16_t QMath_Add(int16_t a, int16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    /* UB!!! */
    int16_t result = a + b;
    return result;
}

/* Add with saturation */
int16_t QMath_AddSat(int16_t a, int16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    int32_t result32 = (int32_t)a + (int32_t)b;

    if(result32 > INT16_MAX)
    {
        result32 = INT16_MAX;
    }
    else if(result32 < INT16_MIN)
    {
        result32 = INT16_MIN;
    }
    else
    {
        /* nothing */
    }

    int16_t result = (int16_t)result32;
    return result;
}

int16_t QMath_SubSat(int16_t a, int16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    int32_t result32 = (int32_t)a - (int32_t)b;

    if(result32 > INT16_MAX)
    {
        result32 = INT16_MAX;
    }
    else if(result32 < INT16_MIN)
    {
        result32 = INT16_MIN;
    }
    else
    {
        /* nothing */
    }

    int16_t result = (int16_t)result32;
    return result;
}

int16_t QMath_Sub(int16_t a, int16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    int16_t result = a - b;
    return result;
}

int16_t QMath_Div(int16_t a, int16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    int16_t result = (int16_t)(((int32_t)a << q)/ (int32_t)b);
    return result;
}

uint16_t QMath_UMul(uint16_t a, uint16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);

    uint16_t result = (uint16_t)(((uint32_t)a * (uint32_t)b) >> q);
    return result;
}

uint16_t QMath_UAdd(uint16_t a, uint16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    
    uint16_t result = a + b;
    return result;
}

uint16_t QMath_USub(uint16_t a, uint16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    uint16_t result = a - b;
    return result;
}

uint16_t QMath_UDiv(uint16_t a, uint16_t b, uint16_t q)
{
    ASSERT(q < MAX_Q);
    uint16_t result = (uint16_t)(((uint32_t)a << q)/ (uint32_t)b);
    return result;
}

uint16_t QMath_UAvg(uint16_t p, uint16_t x, uint16_t alpha, uint16_t q)
{
    uint16_t diff = QMath_USub(x, p, q);
    uint16_t z = QMath_UMul(alpha, diff, q);

    uint16_t y = QMath_USub(x, z, q);

    return y;
}

