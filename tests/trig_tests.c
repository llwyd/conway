#include "trig.h"
#include "unity.h"

void test_TRIG_ATan20()
{
    TEST_ASSERT_TRUE(true);
    point_t a = {.x = 5, .y = 5};
    point_t b = {.x = 0, .y = 0};

    uint8_t angle = TRIG_ATan2(&a, &b);

    TEST_ASSERT_EQUAL_UINT8(0, angle);
}

extern void TRIG_TestsRun(void)
{
    RUN_TEST(test_TRIG_ATan20) ;
}
