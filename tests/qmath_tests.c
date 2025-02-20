#include "unity.h"
#include "qmath.h"
#include <math.h>

static void test_QMATH_MUL1x1(void)
{
    uint16_t q = 15;

    int16_t a = 0x7FFF;
    int16_t b = 0x7FFF;

    int16_t result = QMath_Mul(a,b,q);

    TEST_ASSERT_EQUAL_HEX16(0x7FFE, result);
}

static void test_QMATH_MUL1x_1(void)
{
    uint16_t q = 15;

    int16_t a = 0x7FFF;
    int16_t b = 0x8000;

    int16_t result = QMath_Mul(a,b,q);

    TEST_ASSERT_EQUAL_HEX16(0x8001, result);
}

extern void QMATH_TestsRun(void)
{
    RUN_TEST(test_QMATH_MUL1x1);
    RUN_TEST(test_QMATH_MUL1x_1);
}
