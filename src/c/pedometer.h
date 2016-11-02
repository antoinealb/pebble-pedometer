#ifndef PEDOMETER_H
#define PEDOMETER_H

#ifdef __cplusplus
extern "C" {
#endif

/** Enum used to store the state of the edge detection.
 *
 * @note Should not be used directly, but is required by the algorithms
 * internal. */
typedef enum {
    PEDOMETER_ACC_RISING,
    PEDOMETER_ACC_FALLING
} pedometer_state_t;

/** Structure encapsulating all data required by a pedometer algorithm
 * instance. */
typedef struct {
    float *filter_buffer;
    unsigned int filter_buffer_sz;

    float *threshold_buffer;
    unsigned int threshold_buffer_sz;

    pedometer_state_t state;

    unsigned int steps;
    float hysteresis;
} pedometer_t;

/** Inits a pedometer object.
 *
 * @param [in] filter_buffer, filter_buffer_sz The buffer to use for the
 * lowpass filter.
 * @param [in] threshold_buffer, threshold_buffer_sz The buffer to use for the
 * adaptive threshold
 *
 * @note The size of both buffer can be tuned to change the frequency response
 * of the pedometer.
 */
void pedometer_init(pedometer_t *p,
                    float *filter_buffer, unsigned int filter_buffer_sz,
                    float *threshold_buffer, unsigned int threshold_buffer_sz);


/** Process a single acceleration sample. */
void pedometer_process(pedometer_t *p, float sample);

/** Returns the number of steps by the given pedometer. */
unsigned int pedometer_get_step_count(pedometer_t *p);

#ifdef __cplusplus
}
#endif

#endif
