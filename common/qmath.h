#ifndef QMATH_
#define QMATH_

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

uint16_t QMath_Mul(uint16_t a, uint16_t b, uint16_t q);
uint16_t QMath_Add(uint16_t a, uint16_t b, uint16_t q);
uint16_t QMath_Sub(uint16_t a, uint16_t b, uint16_t q);
uint16_t QMath_Div(uint16_t a, uint16_t b, uint16_t q);

#endif /* QMATH */
