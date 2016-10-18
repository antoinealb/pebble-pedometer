#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "adaptive_threshold.h"

TEST_GROUP(AdaptiveThresholdTestGroup)
{
    float buffer[10];
    void setup(void)
    {
        memset(buffer, 0, sizeof(buffer));
    }
};

TEST(AdaptiveThresholdTestGroup, ComputesAdaptiveThreshold)
{
    float val;
    /* Checks that the adaptive threshold is located at the average between the
     * min and max value in the buffer. */
    val = adaptive_threshold_process(buffer, 10, 0.5);

    DOUBLES_EQUAL(0.25, val, 0.01);

    val = adaptive_threshold_process(buffer, 10, -1.);

    DOUBLES_EQUAL(-0.25, val, 0.01);
}
