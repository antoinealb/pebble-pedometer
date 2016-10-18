#ifndef PEDOMETER_H
#define PEDOMETER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PEDOMETER_ACC_RISING,
    PEDOMETER_ACC_FALLING
} pedometer_state_t;

typedef struct {
    float *filter_buffer;
    unsigned int filter_buffer_sz;
    float *threshold_buffer;
    unsigned int threshold_buffer_sz;
    pedometer_state_t state;
    unsigned int steps;
    float hysteresis;
} pedometer_t;

void pedometer_init(pedometer_t *p,
                    float *filter_buffer, unsigned int filter_buffer_sz,
                    float *threshold_buffer, unsigned int threshold_buffer_sz);


void pedometer_process(pedometer_t *p, float sample);

unsigned int pedometer_get_step_count(pedometer_t *p);

#ifdef __cplusplus
}
#endif

#endif
