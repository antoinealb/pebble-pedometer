#include <string.h>
#include "moving_window.h"

void moving_window_append(float *buffer, unsigned int buffer_sz, float value)
{
    /* Discards oldest element. */
    memmove(buffer, &buffer[1], (buffer_sz - 1) * sizeof(float));

    /* Appends new element. */
    buffer[buffer_sz - 1] = value;
}
