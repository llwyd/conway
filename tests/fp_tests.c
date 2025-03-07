#include "trig.h"
#include "unity.h"
#include "qmath.h"
#include <math.h>

static void test_FP_2_DisplayN(void)
{
    float32_t xf = 0.0;
    float32_t yf = -1.0;

    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    TEST_ASSERT_EQUAL_INT16(0, p.x);
    TEST_ASSERT_EQUAL_INT16(0x8000, p.y);

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);


    TEST_ASSERT_EQUAL_UINT8(64, pd.x);
    TEST_ASSERT_EQUAL_UINT8(0, pd.y);
}

static void test_FP_2_DisplayE(void)
{
    float32_t xf = (float32_t)0.99999;
    float32_t yf = 0.0;
    
    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(127, pd.x);
    TEST_ASSERT_EQUAL_UINT8(32, pd.y);
}

static void test_FP_2_DisplayS(void)
{
    float32_t xf = 0.0;
    float32_t yf = (float32_t)0.99999;
    
    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(64, pd.x);
    TEST_ASSERT_EQUAL_UINT8(63, pd.y);
}

static void test_FP_2_DisplayW(void)
{
    float32_t xf = -1.0;
    float32_t yf = 0.0;
    
    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(0, pd.x);
    TEST_ASSERT_EQUAL_UINT8(32, pd.y);
}

static void test_FP_2_DisplaySE_2(void)
{
    float32_t xf = 0.5;
    float32_t yf = 0.5;
    
    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(96, pd.x);
    TEST_ASSERT_EQUAL_UINT8(48, pd.y);
}

static void test_FP_2_DisplaySW(void)
{
    float32_t xf = -1.0;
    float32_t yf = (float32_t)0.99999;
    
    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(0, pd.x);
    TEST_ASSERT_EQUAL_UINT8(63, pd.y);
}

static void test_FP_2_DisplayNW_2(void)
{
    float xf = -0.5;
    float yf = -0.5;

    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(32, pd.x);
    TEST_ASSERT_EQUAL_UINT8(16, pd.y);
}

static void test_FP_2_DisplayNW_4(void)
{
    float xf = -0.75;
    float yf = -0.75;

    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(16, pd.x);
    TEST_ASSERT_EQUAL_UINT8(8, pd.y);
}

static void test_FP_2_DisplayNW(void)
{
    float xf = -1;
    float yf = -1;

    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(0, pd.x);
    TEST_ASSERT_EQUAL_UINT8(0, pd.y);
}

static void test_FP_2_DisplayCentre(void)
{
    float xf = 0.0;
    float yf = 0.0;

    pointf16_t p =
    {
        .x = Q_FLOAT_TO_Q(xf, Q_NUM15),
        .y = Q_FLOAT_TO_Q(yf, Q_NUM15),
    };

    point_t pd;
    pd.x = QMath_Int16ToUInt8(p.x, 1U);
    pd.y = QMath_Int16ToUInt8(p.y, 2U);

    TEST_ASSERT_EQUAL_UINT8(64, pd.x);
    TEST_ASSERT_EQUAL_UINT8(32, pd.y);
}


extern void FP_TestsRun(void)
{
    RUN_TEST(test_FP_2_DisplayN);
    RUN_TEST(test_FP_2_DisplayE);
    RUN_TEST(test_FP_2_DisplaySE_2);
    RUN_TEST(test_FP_2_DisplayS);
    RUN_TEST(test_FP_2_DisplaySW);
    RUN_TEST(test_FP_2_DisplayW);
    RUN_TEST(test_FP_2_DisplayNW);
    RUN_TEST(test_FP_2_DisplayNW_2);
    RUN_TEST(test_FP_2_DisplayNW_4);
    RUN_TEST(test_FP_2_DisplayCentre);
}
