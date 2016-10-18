#include "lowpass.h"
#include "moving_window.h"

float lowpass_process(float *buffer, unsigned int buffer_sz, float sample)
{
    float sum = 0.;
    unsigned int i;

    moving_window_append(buffer, buffer_sz, sample);

    /* Computes the average over the whole buffer. */
    for (i = 0; i < buffer_sz; i++) {
        sum += buffer[i];
    }

    return sum / buffer_sz;
}
