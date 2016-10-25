#ifndef ADAPTIVE_THRESHOLD_H
#define ADAPTIVE_THRESHOLD_H

#ifdef __cplusplus
extern "C" {
#endif

/** Process a single value for the adaptive threshold and returns the processed value.
 *
 * @param [in] buffer The float array to use for storing the elements.
 * @param [in] nel The number of elements in the buffer, in bytes.
 * @param [in] value The value to append.
 */
float adaptive_threshold_process(float *buffer, unsigned int buffer_sz, float sample);

#ifdef __cplusplus
}
#endif

#endif
