#include "trig.h"
#include "unity.h"
#include "qmath.h"
#include <math.h>

typedef float float32_t;

static float32_t Bin2Deg(uint8_t x)
{
    float result = (1.0 / (float)(1 << 7)) * (180.0/ M_PI) * (M_PI) * (float)x;

    return result;
}

static void test_TRIG_ATan2_0Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = 
    {
        .x = Q_FLOAT_TO_Q(0.5, Q_NUM), 
        .y = 0
    };
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b); 
    TEST_ASSERT_UINT8_WITHIN(2, 0 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 0.0, f_angle);
}

static void test_TRIG_ATan2_45Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = 
    {
        .x = Q_FLOAT_TO_Q(0.5, Q_NUM), 
        .y = Q_FLOAT_TO_Q(0.5, Q_NUM)
    };
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 1 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 45.0, f_angle);
}

static void test_TRIG_ATan2_90Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = 0, .y = Q_FLOAT_TO_Q(0.5, Q_NUM)};
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 2 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 90.0, f_angle);
}

static void test_TRIG_ATan2_135Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x =Q_FLOAT_TO_Q(-0.5, Q_NUM), .y = Q_FLOAT_TO_Q(0.5, Q_NUM)};
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 3 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 135.0, f_angle);
}

static void test_TRIG_ATan2_180Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = Q_FLOAT_TO_Q(-0.5, Q_NUM), .y = 0};
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 4 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 180.0, f_angle);
}

static void test_TRIG_ATan2_225Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = Q_FLOAT_TO_Q(-0.5, Q_NUM), .y = Q_FLOAT_TO_Q(-0.5, Q_NUM)};
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 5 * (1 << 5), angle);


    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 225.0, f_angle);
}

static void test_TRIG_ATan2_270Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = 0, .y = Q_FLOAT_TO_Q(-0.5, Q_NUM)};
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 6 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 270.0, f_angle);
}

static void test_TRIG_ATan2_315Deg()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = Q_FLOAT_TO_Q(0.5, Q_NUM), .y = Q_FLOAT_TO_Q(-0.5, Q_NUM)};
    pointf16_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 7 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 315.0, f_angle);
}

static void test_TRIG_ATan2_Extreme_xy()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = Q_FLOAT_TO_Q(0.5, Q_NUM), .y = Q_FLOAT_TO_Q(0.5, Q_NUM)};
    pointf16_t b = {.x = Q_FLOAT_TO_Q(-0.5, Q_NUM), .y = Q_FLOAT_TO_Q(-0.5, Q_NUM)};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 1 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 45.0, f_angle);
}

static void test_TRIG_ATan2_Extreme_xy_2()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = Q_FLOAT_TO_Q(-0.5, Q_NUM), .y = Q_FLOAT_TO_Q(-0.5, Q_NUM)};
    pointf16_t b = {.x = Q_FLOAT_TO_Q(0.5, Q_NUM), .y = Q_FLOAT_TO_Q(0.5, Q_NUM)};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 5 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 225.0, f_angle);
}

static void test_TRIG_ATan2_Extreme_xy_3()
{
    TEST_ASSERT_TRUE(true);
    pointf16_t a = {.x = Q_FLOAT_TO_Q(0.9999, Q_NUM), .y = Q_FLOAT_TO_Q(0.9999, Q_NUM)};
    pointf16_t b = {.x = Q_FLOAT_TO_Q(-1.0, Q_NUM), .y = Q_FLOAT_TO_Q(-1.0, Q_NUM)};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 1 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 45.0, f_angle);
}

static void test_TRIG_SAM_0_90(void)
{
    uint8_t a = DEG_0;
    uint8_t b = DEG_90;

    uint8_t mp = TRIG_SAM(a, b);

    TEST_ASSERT_UINT8_WITHIN(1, DEG_45, mp);
}

static void test_TRIG_SAM_135_225(void)
{
    uint8_t a = DEG_135;
    uint8_t b = DEG_225;

    uint8_t mp = TRIG_SAM(a, b);

    TEST_ASSERT_UINT8_WITHIN(1, DEG_180, mp);
}

static void test_TRIG_SAM_315_45(void)
{
    uint8_t a = DEG_315;
    uint8_t b = DEG_45;

    uint8_t mp = TRIG_SAM(a, b);

    TEST_ASSERT_UINT8_WITHIN(2, DEG_0, mp);
}

static void test_TRIG_SAM_270_0(void)
{
    uint8_t a = DEG_270;
    uint8_t b = DEG_0;

    uint8_t mp = TRIG_SAM(a, b);

    TEST_ASSERT_UINT8_WITHIN(2, DEG_315, mp);
}

extern void TRIG_TestsRun(void)
{
    RUN_TEST(test_TRIG_ATan2_0Deg);
    RUN_TEST(test_TRIG_ATan2_45Deg);
    RUN_TEST(test_TRIG_ATan2_90Deg);
    RUN_TEST(test_TRIG_ATan2_135Deg);
    RUN_TEST(test_TRIG_ATan2_180Deg);
    RUN_TEST(test_TRIG_ATan2_225Deg);
    RUN_TEST(test_TRIG_ATan2_270Deg);
    RUN_TEST(test_TRIG_ATan2_315Deg);
    RUN_TEST(test_TRIG_ATan2_Extreme_xy);
    RUN_TEST(test_TRIG_ATan2_Extreme_xy_2);
    RUN_TEST(test_TRIG_ATan2_Extreme_xy_3);
    
    RUN_TEST(test_TRIG_SAM_0_90);
    RUN_TEST(test_TRIG_SAM_135_225);
    RUN_TEST(test_TRIG_SAM_315_45);
    RUN_TEST(test_TRIG_SAM_270_0);
}
