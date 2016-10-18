#pragma once
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
    int *filter_buffer;
    unsigned long int filter_buffer_sz;
    int *threshold_buffer;
    unsigned long int threshold_buffer_sz;
    pedometer_state_t state;
    unsigned int steps;
    int hysteresis;
} pedometer_t;

void pedometer_init(pedometer_t *p,
                    int *filter_buffer, unsigned long int filter_buffer_sz,
                    int *threshold_buffer, unsigned long int threshold_buffer_sz);


void pedometer_process(pedometer_t *p, int sample);

unsigned int pedometer_get_step_count(pedometer_t *p);
  
void pedometer_reset(pedometer_t *p);

pedometer_t meter;

#ifdef __cplusplus
}
#endif

#endif