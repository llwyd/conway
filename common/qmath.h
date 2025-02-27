#ifndef QMATH_
#define QMATH_

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef float float32_t;

#define Q_UPSCALE(X,Y) ((int16_t)(((uint16_t)(X)) << (Y)))
#define Q_DNSCALE(X,Y) ((uint8_t)(((X)) >> (Y)))

#define QNUM_DNSCALE(X,Y,Z) ((((X)) >> ((Y) - (Z))))
#define QNUM_UPSCALE(X,Y,Z) ((((X)) << ((Y) - (Z))))

#define Q_UUPSCALE(X,Y) ((uint16_t)(((uint16_t)(X)) << (Y)))
#define Q_UDNSCALE(X,Y) ((uint8_t)(((X)) >> (Y)))

#define Q_NUM (15)
#define Q_SCALE (Q_NUM - 8U)

#define Q_NUM15 (15)
#define Q_NUM7 (7)

#define Q_FLOAT_TO_Q(X, Q) (int16_t)((X) * (float32_t)(1 << (Q)))

int16_t QMath_Mul(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Add(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Sub(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Div(int16_t a, int16_t b, uint16_t q);

uint16_t QMath_UMul(uint16_t a, uint16_t b, uint16_t q);
uint16_t QMath_UAdd(uint16_t a, uint16_t b, uint16_t q);
uint16_t QMath_USub(uint16_t a, uint16_t b, uint16_t q);
uint16_t QMath_UDiv(uint16_t a, uint16_t b, uint16_t q);

uint16_t QMath_UAvg(uint16_t p, uint16_t x, uint16_t alpha, uint16_t q);

#endif /* QMATH */

