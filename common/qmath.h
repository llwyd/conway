#ifndef QMATH_
#define QMATH_

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

int16_t QMath_Mul(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Add(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Sub(int16_t a, int16_t b, uint16_t q);
int16_t QMath_Div(int16_t a, int16_t b, uint16_t q);

#endif /* QMATH */
