#include <math.h>
#include "adaptive_threshold.h"
#include "moving_window.h"

int adaptive_threshold_process(int *buffer, unsigned long int buffer_sz, int sample)
{
    int min, max;
    unsigned long int i;

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


