#include "lowpass.h"
#include "moving_window.h"

int lowpass_process(int *buffer, unsigned long int buffer_sz, int sample)
{
    int sum = 0.;
    unsigned long int i;

    moving_window_append(buffer, buffer_sz, sample);

    /* Computes the average over the whole buffer. */
    for (i = 0; i < buffer_sz; i++) {
        sum += buffer[i];
    }

    return sum / buffer_sz;
}
