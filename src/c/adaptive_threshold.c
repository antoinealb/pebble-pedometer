#include <math.h>
#include "adaptive_threshold.h"
#include "moving_window.h"

float adaptive_threshold_process(float *buffer, unsigned int buffer_sz, float sample)
{
    float min, max;
    unsigned int i;

    moving_window_append(buffer, buffer_sz, sample);

    min = buffer[0];
    for (i = 0; i < buffer_sz; i++) {
        min = fmin(min, buffer[i]);
    }

    max = buffer[0];
    for (i = 0; i < buffer_sz; i++) {
        max = fmax(max, buffer[i]);
    }

    return 0.5 * (max - min) + min;
}
