#include <string.h>
#include "moving_window.h"

void moving_window_append(int *buffer, unsigned long int buffer_sz, int value)
{
    /* Discards oldest element. */
    memmove(buffer, &buffer[1], (buffer_sz - 1) * sizeof(int));

    /* Appends new element. */
    buffer[buffer_sz - 1] = value;
}
