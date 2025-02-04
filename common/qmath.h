#ifndef QMATH_
#define QMATH_

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define Q_UPSCALE(X,Y) ((int16_t)(((uint16_t)(X)) << (Y)));
#define Q_DNSCALE(X,Y) ((uint8_t)(((X)) >> (Y)));

int16_t QMath_Mul(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Add(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Sub(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Div(int16_t a, int16_t b, uint16_t q);

#endif /* QMATH */
