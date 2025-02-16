#include "trig.h"
#include "unity.h"
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
    point_t a = {.x = 5, .y = 0};
    point_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b); 
    TEST_ASSERT_UINT8_WITHIN(2, 0 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 0.0, f_angle);
}

static void test_TRIG_ATan2_45Deg()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 5, .y = 5};
    point_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 1 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 45.0, f_angle);
}

static void test_TRIG_ATan2_90Deg()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 0, .y = 5};
    point_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 2 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 90.0, f_angle);
}

static void test_TRIG_ATan2_135Deg()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 0, .y = 5};
    point_t b = {.x = 5, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 3 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 135.0, f_angle);
}

static void test_TRIG_ATan2_180Deg()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 0, .y = 0};
    point_t b = {.x = 5, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 4 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 180.0, f_angle);
}

static void test_TRIG_ATan2_225Deg()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 0, .y = 0};
    point_t b = {.x = 5, .y = 5};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 5 * (1 << 5), angle);


    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 225.0, f_angle);
}

static void test_TRIG_ATan2_270Deg()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 0, .y = 0};
    point_t b = {.x = 0, .y = 5};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 6 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 270.0, f_angle);
}

static void test_TRIG_ATan2_315Deg()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 5, .y = 0};
    point_t b = {.x = 0, .y = 5};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_UINT8_WITHIN(2, 7 * (1 << 5), angle);

    float32_t f_angle = Bin2Deg(angle);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 315.0, f_angle);
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
}
