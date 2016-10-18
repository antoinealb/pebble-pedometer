#pragma once
#ifndef MOVING_WINDOW_H
#define MOVING_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

/** Appends an element to the end of a moving window.
 *
 * @param [in] buffer The int array to use for storing the elements.
 * @param [in] nel The number of elements in the buffer, in bytes.
 * @param [in] value The value to append.
 */
void moving_window_append(int *buffer, unsigned long int buffer_sz, int value);


#ifdef __cplusplus
}
#endif

#endif
