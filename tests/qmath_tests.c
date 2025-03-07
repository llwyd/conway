#include "unity.h"
#include "qmath.h"
#include <math.h>

static void test_QMATH_Q15_Sanity(void)
{
    float32_t xf = 0.0;
    int16_t x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_EQUAL_INT16(0x0000, x);
    
    xf = (float32_t)0.99999;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_EQUAL_INT16(0x7FFF, x); 
    
    xf = 0.5;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_INT16_WITHIN(5, 0x3FFF, x);
    
    xf = 0.25;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_INT16_WITHIN(5, 0x1FFF, x);
    
    xf = 0.125;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_INT16_WITHIN(5, 0x0FFF, x);
    
    xf = -1.0;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_EQUAL_INT16(0x8000, x);
    
    xf = -0.5;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_INT16_WITHIN(5, 0xC000, x);
    
    xf = -0.25;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_INT16_WITHIN(5, 0xe000, x);
    
    xf = -0.125;
    x = Q_FLOAT_TO_Q(xf, Q_NUM15);
    TEST_ASSERT_INT16_WITHIN(5, 0xF000, x);
}

static void test_QMATH_MUL1x1(void)
{
    int16_t a = Q_FLOAT_TO_Q(0.99999, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(0.99999, Q_NUM);
    int16_t result = QMath_Mul(a,b,Q_NUM);

    TEST_ASSERT_EQUAL_HEX16(0x7FFE, result);
}

static void test_QMATH_MUL1x_1(void)
{
    int16_t a = Q_FLOAT_TO_Q(0.99999, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(-1.0, Q_NUM);
    int16_t result = QMath_Mul(a,b,Q_NUM);

    int16_t exp = Q_FLOAT_TO_Q(-0.9999, Q_NUM);
    TEST_ASSERT_INT16_WITHIN(4, exp, result);
}

static void test_QMATH_MUL_1x_1(void)
{
    int16_t a = Q_FLOAT_TO_Q(-1.0, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(-1.0, Q_NUM);
    int16_t result = QMath_Mul(a,b,Q_NUM);

    int16_t exp = Q_FLOAT_TO_Q(-1.0, Q_NUM);
    TEST_ASSERT_INT16_WITHIN(4, exp, result);
}

static void test_QMATH_MUL1x05(void)
{
    int16_t a = Q_FLOAT_TO_Q(0.9999, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(0.5, Q_NUM);
    int16_t result = QMath_Mul(a,b,Q_NUM);

    int16_t exp = Q_FLOAT_TO_Q(0.5, Q_NUM);
    TEST_ASSERT_INT16_WITHIN(4, exp, result);
}

static void test_QMATH_MUL05x05(void)
{
    int16_t a = Q_FLOAT_TO_Q(0.5, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(0.5, Q_NUM);
    int16_t result = QMath_Mul(a,b,Q_NUM);

    int16_t exp = Q_FLOAT_TO_Q(0.25, Q_NUM);
    TEST_ASSERT_INT16_WITHIN(1, exp, result);
}

static void test_QMATH_MUL05x_05(void)
{
    int16_t a = Q_FLOAT_TO_Q(0.5, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(-0.5, Q_NUM);
    int16_t result = QMath_Mul(a,b,Q_NUM);

    int16_t exp = Q_FLOAT_TO_Q(-0.25, Q_NUM);
    TEST_ASSERT_INT16_WITHIN(1, exp, result);
}

static void test_QMATH_ADD25x25(void)
{
    int16_t a = Q_FLOAT_TO_Q(0.25, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(0.25, Q_NUM);
    int16_t result = QMath_Add(a,b,Q_NUM);

    int16_t exp = Q_FLOAT_TO_Q(0.5, Q_NUM);
    TEST_ASSERT_INT16_WITHIN(1, exp, result);
}

static void test_QMATH_ADD_25x25(void)
{
    int16_t a = Q_FLOAT_TO_Q(-0.25, Q_NUM);
    int16_t b = Q_FLOAT_TO_Q(0.25, Q_NUM);
    int16_t result = QMath_Add(a,b,Q_NUM);

    int16_t exp = Q_FLOAT_TO_Q(0.0, Q_NUM);
    TEST_ASSERT_INT16_WITHIN(1, exp, result);
}

static void test_QMATH_Avg1(void)
{
    uint16_t q = 15;

    uint16_t a = 0x7FFF;
    uint16_t alpha = 0x3FFF;
    uint16_t y = 0;
    y = QMath_UAvg(y,a,alpha,q);

    TEST_ASSERT_UINT16_WITHIN(10U, a >> 1, y);
}

static void test_QMATH_Avg2(void)
{
    uint16_t q = 15;

    uint16_t a = 0x7FFF;
    uint16_t alpha = 0x3FFF;
    uint16_t y = 0;
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    
    TEST_ASSERT_UINT16_WITHIN(100U, a, y);
}

static void test_QMATH_Avg3(void)
{
    uint16_t q = 15;

    uint16_t a = 0x1234;
    uint16_t alpha = 0x0987;
    uint16_t y = 0;
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    y = QMath_UAvg(y,a,alpha,q);
    
    TEST_ASSERT_UINT16_WITHIN(1U, a, y);
}

static void test_QMATH_Avg4(void)
{
    uint16_t q = 15;
    uint8_t data[] = {1,2,3,4,5,6,7,8};
    
    uint8_t avg = 0;
    for(uint32_t idx = 0; idx < 8; idx++)
    {
        avg += data[idx];
    }
    avg /= 8U;

    uint16_t alpha = 0x7000;
    uint16_t y = 0;
    
    y = QMath_UAvg(y,Q_UUPSCALE(data[0], Q_SCALE),alpha,q);
    y = QMath_UAvg(y,Q_UUPSCALE(data[1], Q_SCALE),alpha,q);
    y = QMath_UAvg(y,Q_UUPSCALE(data[2], Q_SCALE),alpha,q);
    y = QMath_UAvg(y,Q_UUPSCALE(data[3], Q_SCALE),alpha,q);
    y = QMath_UAvg(y,Q_UUPSCALE(data[4], Q_SCALE),alpha,q);
    y = QMath_UAvg(y,Q_UUPSCALE(data[5], Q_SCALE),alpha,q);
    y = QMath_UAvg(y,Q_UUPSCALE(data[6], Q_SCALE),alpha,q);
    y = QMath_UAvg(y,Q_UUPSCALE(data[7], Q_SCALE),alpha,q);
    
    uint8_t y8 = Q_UDNSCALE(y, Q_SCALE);

    TEST_ASSERT_UINT8_WITHIN(1U, avg, y8);
}

extern void QMATH_TestsRun(void)
{
    RUN_TEST(test_QMATH_Q15_Sanity);
    RUN_TEST(test_QMATH_MUL1x1);
    RUN_TEST(test_QMATH_MUL1x_1);
    RUN_TEST(test_QMATH_MUL_1x_1);
    RUN_TEST(test_QMATH_MUL1x05);
    RUN_TEST(test_QMATH_MUL05x05);
    RUN_TEST(test_QMATH_MUL05x_05);
    RUN_TEST(test_QMATH_ADD25x25);
    RUN_TEST(test_QMATH_ADD_25x25);
    RUN_TEST(test_QMATH_Avg1);
    RUN_TEST(test_QMATH_Avg2);
    RUN_TEST(test_QMATH_Avg3);
    RUN_TEST(test_QMATH_Avg4);
}
