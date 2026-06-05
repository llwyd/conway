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

static int16_t calculate_qaverage( int16_t * data, uint32_t num, int16_t alpha, int16_t error)
{
    TEST_ASSERT_TRUE(num > 0u);
    TEST_ASSERT_TRUE(alpha > 0);
    TEST_ASSERT_TRUE(Q_NUM == 15); 
    int32_t avg = 0;
    int16_t q_avg = 0;
    for(uint32_t idx = 0; idx < num; idx++)
    {
        q_avg = QMath_Avg(q_avg, data[idx],alpha, Q_NUM);
        avg += (int32_t)data[idx];
    }
    avg /= num;

    int16_t error_val = QMath_Mul(error, avg, Q_NUM);

    TEST_ASSERT_INT16_WITHIN(error_val, (int16_t)avg, q_avg);
}

#define TEST_ALPHA (0x3FFF)

static void test_QMATH_Avg1(void)
{

    int16_t x = 0x1234;

    /* 0.5 */
    int16_t alpha = TEST_ALPHA;
    
    TEST_ASSERT_TRUE(alpha > 0);
    TEST_ASSERT_TRUE(Q_NUM == 15); 
 
    int16_t q_avg = 0;
    q_avg = QMath_Avg(q_avg, x, alpha, Q_NUM);

    TEST_ASSERT_INT16_WITHIN(1U, x >> 1, q_avg);
}

static void test_QMATH_Avg2(void)
{
    int16_t x[10] = {0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234};
    int16_t alpha = TEST_ALPHA;

    /* Error within 1% if value consistent */
    calculate_qaverage(x, 10u, alpha, Q_FLOAT_TO_Q(0.01, Q_NUM) );
}

static void test_QMATH_Avg3(void)
{
    int16_t x[5] = {0x1234,0x1234,0x1234,0x1234,0x1234};
    int16_t alpha = TEST_ALPHA;
    /* Error within 1% if value consistent */
    calculate_qaverage(x, 5u, alpha, Q_FLOAT_TO_Q(0.05, Q_NUM));
}

static void test_QMATH_Avg4(void)
{
    int16_t x[16] = {10713,  2266, 30213, 12240,  9945, 26832, 19317, 21910, 10481, 27502,  2459, 22125,  3268, 14945, 15292,  6970};
    int16_t alpha = TEST_ALPHA;

    /* Random values, error within 50% */
    calculate_qaverage(x, 16u, alpha,Q_FLOAT_TO_Q(0.5, Q_NUM));
}

static void test_QMATH_Avg5(void)
{
    int16_t x[8] = {3049, 3180,  462,  915, 2055,  335, 2086, 2914};
    int16_t alpha = TEST_ALPHA;

    /* Random values within 10% of unsigned range, error within 20% */
    calculate_qaverage(x, 8u, alpha,Q_FLOAT_TO_Q(0.2, Q_NUM));
}

static void test_QMATH_Avg6(void)
{
    int16_t x[8] = {1061,   77, 5261, 2605, 5165, 5645, 5837, 5045};
    int16_t alpha = TEST_ALPHA;

    /* Random values within 20% of unsigned range, error within 35% */
    calculate_qaverage(x, 8u, alpha,Q_FLOAT_TO_Q(0.35, Q_NUM));
}

/* np.random.randint(0,np.iinfo(np.int16).max * 0.5,8) */
static void test_QMATH_Avg7(void)
{
    int16_t x[8] = {643, 8711, 3487, 3675, 6184, 9687, 7530, 7820};
    int16_t y[8] = { 2654,  8870,  5990, 13745,  5336,  8264, 15695,  7982};
    int16_t z[8] = {3824,  9606,   468, 11012, 15802, 14620, 10323,  9183};
    int16_t alpha = TEST_ALPHA;

    /* Random values within 50% of unsigned range, error within 35% */
    calculate_qaverage(x, 8u, alpha,Q_FLOAT_TO_Q(0.35, Q_NUM));
    calculate_qaverage(y, 8u, alpha,Q_FLOAT_TO_Q(0.35, Q_NUM));
    calculate_qaverage(z, 8u, alpha,Q_FLOAT_TO_Q(0.35, Q_NUM));
}

/* np.random.randint(0,np.iinfo(np.int16).max * 0.5,8) */
static void test_QMATH_Avg8(void)
{
    int16_t a[8] = {15528,   697, 19235, 11483,  3499, 22476,  9341,  4999};
    int16_t b[8] = {26840, 11063,  4253,  4490,  2964, 16817,  4812, 30497};
    int16_t c[8] = { 6729,   672,  5820,  4697,  6245, 25993, 31553, 11459};
    int16_t d[8] = {22227,  3076, 21898, 23628, 29246, 10934, 15118, 19926};
    int16_t x[8] = {12092,  9460, 15579, 12490,  2985,  4453, 10384, 32111};
    int16_t y[8] = {21670, 29159, 18329, 11796, 12286, 16828, 22261, 17409};
    int16_t z[8] = {21670, 29159, 18329, 11796, 12286, 16828, 22261, 17409};
    int16_t alpha = TEST_ALPHA;

    float32_t error = 0.65;
    /* Random values within 100% of unsigned range, error within 65% */
    calculate_qaverage(a, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(b, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(c, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(d, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(x, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(y, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(z, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
}

static void test_QMATH_Avg9(void)
{
    int16_t a[8] = {-15528,   -697, -19235, -11483,  -3499, -22476,  -9341,  -4999};
    int16_t b[8] = {-26840, -11063,  -4253,  -4490,  -2964, -16817,  -4812, -30497};
    int16_t c[8] = { -6729,   -672,  -5820,  -4697,  -6245, -25993, -31553, -11459};
    int16_t d[8] = {-22227,  -3076, -21898, -23628, -29246, -10934, -15118, -19926};
    int16_t x[8] = {-12092,  -9460, -15579, -12490,  -2985,  -4453, -10384, -32111};
    int16_t y[8] = {-21670, -29159, -18329, -11796, -12286, -16828, -22261, -17409};
    int16_t z[8] = {-21670, -29159, -18329, -11796, -12286, -16828, -22261, -17409};
    int16_t alpha = 0x3FFF;

    float32_t error = TEST_ALPHA;
    /* Random values within 100% of unsigned range, error within 65% */
    calculate_qaverage(a, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(b, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(c, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(d, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(x, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(y, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
    calculate_qaverage(z, 8u, alpha,Q_FLOAT_TO_Q(error, Q_NUM));
}

static int16_t calculate_average_angle( uint8_t * data, uint32_t num, int16_t alpha, uint8_t error)
{
    TEST_ASSERT_TRUE(num > 0u);
    TEST_ASSERT_TRUE(alpha > 0);
    TEST_ASSERT_TRUE(Q_NUM == 15); 
    uint32_t avg = 0;
    int16_t q_avg = 0;
    for(uint32_t idx = 0; idx < num; idx++)
    {
        int16_t data16 = (int16_t)data[idx];
        q_avg = QMath_Avg(q_avg, data16,alpha, Q_NUM);
        avg += (int32_t)data[idx];
    } 
    avg /= num;
    TEST_ASSERT_TRUE(avg <= UINT8_MAX);
    
    uint8_t avg8 = (uint8_t)avg;


    TEST_ASSERT_UINT8_WITHIN(error, avg, q_avg);
}

static void test_QMATH_AvgAngle(void)
{
    uint8_t x = 0x12;

    int16_t x16 = (int16_t)0x12;
    int16_t alpha = TEST_ALPHA;
    
    int16_t q_avg = 0;
    q_avg = QMath_Avg(q_avg, x16,alpha, Q_NUM);

    TEST_ASSERT_TRUE(q_avg <= UINT8_MAX);
    uint8_t q_avg8 = (uint8_t)q_avg;

    TEST_ASSERT_INT16_WITHIN(1U, x >> 1, q_avg8);
}

static void test_QMATH_AvgAngle2(void)
{
    uint8_t x[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
    int16_t alpha = TEST_ALPHA;

    calculate_average_angle(x, 8u, alpha, 1u );
}

static void test_QMATH_AvgAngle3(void)
{
    uint8_t x[8] = {24, 47, 26, 25, 48, 46,  6,  1};
    uint8_t y[8] = {14,  2, 20, 24, 15,  2,  5, 18};
    uint8_t z[8] = {4,  3,  2,  9,  6,  5,  3, 10};
    int16_t alpha = TEST_ALPHA;

    calculate_average_angle(x, 8u, alpha, 25 ); // 25%
    calculate_average_angle(y, 8u, alpha, 25 );  // 10%
    calculate_average_angle(z, 8u, alpha, 25 );  //  5%
}

static void test_QMATH_AvgAngle4(void)
{
    uint8_t x[8] = {23, 29, 23,  4, 20, 57, 56, 43};
    uint8_t y[8] = {3, 19, 19, 12,  6, 23, 12,  3};
    uint8_t z[8] = {11,  6,  2,  9,  3, 11,  6,  3};
    int16_t alpha = TEST_ALPHA;

    calculate_average_angle(x, 8u, alpha, 25 ); // 25%
    calculate_average_angle(y, 8u, alpha, 25 );  // 10%
    calculate_average_angle(z, 8u, alpha, 25 );  //  5%
}

static void test_QMATH_AvgAngle5(void)
{
    uint8_t x[8] = {156,  18,  82,  71, 135,  90, 253, 160};
    uint8_t y[8] = {248, 103, 243,  59,  23, 133,  50, 212};
    uint8_t z[8] = {140,  12,  15, 241,  42,  41, 188, 118};
    int16_t alpha = TEST_ALPHA;

    calculate_average_angle(x, 8u, alpha, 50 );
    calculate_average_angle(y, 8u, alpha, 50 );
    calculate_average_angle(z, 8u, alpha, 50 ); 
}

static void test_QMATH_AvgAngle6(void)
{
    uint8_t x[8] = {54, 254,  93, 132, 198, 190, 165,   5};
    uint8_t y[8] = {7,  35, 145, 174,  60,  57,  80,  37};
    uint8_t z[8] = {178, 142,  73,  69, 208, 241, 140, 163};
    int16_t alpha = TEST_ALPHA;

    calculate_average_angle(x, 8u, alpha, 50 );
    calculate_average_angle(y, 8u, alpha, 50 );
    calculate_average_angle(z, 8u, alpha, 50 ); 
}

static void test_QMATH_AvgUnused(void)
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
    RUN_TEST(test_QMATH_Avg5);
    RUN_TEST(test_QMATH_Avg6);
    RUN_TEST(test_QMATH_Avg7);
    RUN_TEST(test_QMATH_Avg8);
    RUN_TEST(test_QMATH_Avg9);
    RUN_TEST(test_QMATH_AvgAngle);
    RUN_TEST(test_QMATH_AvgAngle2);
    RUN_TEST(test_QMATH_AvgAngle3);
    RUN_TEST(test_QMATH_AvgAngle4);
    RUN_TEST(test_QMATH_AvgAngle5);
    RUN_TEST(test_QMATH_AvgAngle6);
}
