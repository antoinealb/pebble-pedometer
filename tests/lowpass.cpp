#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "lowpass.h"

TEST_GROUP(LowPassTestGroup)
{
    float buffer[10];

    void setup(void)
    {
        memset(buffer, 0, sizeof(buffer));
    }
};

TEST(LowPassTestGroup, CanLowPass)
{
    float val;
    val = lowpass_process(buffer, 10, 1.);
    DOUBLES_EQUAL(0.1, val, 0.01);

    val = lowpass_process(buffer, 10, 1.);
    DOUBLES_EQUAL(0.2, val, 0.01);
}
