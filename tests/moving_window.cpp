#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "moving_window.h"

#define BUF_SIZE 10


TEST_GROUP(MovingWindowTestGroup)
{
    float buffer[BUF_SIZE];

    void setup()
    {
        memset(buffer, 0, sizeof(buffer));
    }
};

TEST(MovingWindowTestGroup, CanAppendSimpleCase)
{
    moving_window_append(buffer, BUF_SIZE, 10.);
    CHECK_EQUAL(10., buffer[BUF_SIZE - 1]);
}

TEST(MovingWindowTestGroup, PreservesHistory)
{
    moving_window_append(buffer, BUF_SIZE, 10.);
    moving_window_append(buffer, BUF_SIZE, 20.);
    CHECK_EQUAL(10., buffer[BUF_SIZE - 2]);
    CHECK_EQUAL(20., buffer[BUF_SIZE - 1]);
}
