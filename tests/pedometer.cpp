#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "pedometer.h"

TEST_GROUP(PedometerTestGroup)
{
    pedometer_t pedometer;
    float filter_buffer[2];
    float threshold_buffer[3];

    void setup()
    {
        memset(&pedometer, 0x55, sizeof(pedometer));
        memset(filter_buffer, 0, sizeof(filter_buffer));
        memset(threshold_buffer, 0, sizeof(threshold_buffer));
        pedometer_init(&pedometer,
                       filter_buffer, 2,
                       threshold_buffer, 3);
    }
};

TEST(PedometerTestGroup, CanInit)
{
    POINTERS_EQUAL(filter_buffer, pedometer.filter_buffer);
    POINTERS_EQUAL(threshold_buffer, pedometer.threshold_buffer);
    CHECK_EQUAL(2, pedometer.filter_buffer_sz);
    CHECK_EQUAL(3, pedometer.threshold_buffer_sz);
    CHECK_EQUAL(PEDOMETER_ACC_RISING, pedometer.state);
    CHECK_EQUAL(0, pedometer_get_step_count(&pedometer));
}

TEST(PedometerTestGroup, CanSetHysteresis)
{
    pedometer_set_hysteresis(&pedometer, 10);
    CHECK_EQUAL(10, pedometer.hysteresis);
}

TEST(PedometerTestGroup, CanReset)
{
    pedometer.steps = 1234;
    pedometer_reset_step_count(&pedometer);
    CHECK_EQUAL(0, pedometer.steps);
}

TEST(PedometerTestGroup, CanProcessRising)
{
    pedometer_process(&pedometer, 1.);

    /* Check that the filtered value is correct. */
    DOUBLES_EQUAL(0.5, pedometer.threshold_buffer[2], 0.1);

    /* Check that the value fell back to falling. */
    CHECK_EQUAL(PEDOMETER_ACC_FALLING, pedometer.state);

    /* Check that we had one more step. */
    CHECK_EQUAL(1, pedometer_get_step_count(&pedometer));
}

TEST(PedometerTestGroup, CanProcessRisingWithHysteresis)
{
    pedometer.hysteresis = 1;
    pedometer_process(&pedometer, 1.);

    /* Check that the value stayed at rising because of the hysteresis. */
    CHECK_EQUAL(PEDOMETER_ACC_RISING, pedometer.state);
    CHECK_EQUAL(0, pedometer_get_step_count(&pedometer));
}

TEST(PedometerTestGroup, CanProcessFalling)
{
    /* arrange, act, assert */
    pedometer.state = PEDOMETER_ACC_FALLING;
    pedometer_process(&pedometer, -1.);
    CHECK_EQUAL(PEDOMETER_ACC_RISING, pedometer.state);
}

TEST(PedometerTestGroup, CanProcessFallingWithHysteresis)
{
    pedometer.state = PEDOMETER_ACC_FALLING;
    pedometer.hysteresis = 2.;

    pedometer_process(&pedometer, -1.);
    CHECK_EQUAL(PEDOMETER_ACC_FALLING, pedometer.state);
}
