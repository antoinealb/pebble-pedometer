#include "pedometer.h"
#include "lowpass.h"
#include "adaptive_threshold.h"

void pedometer_init(pedometer_t *p,
                    float *filter_buffer, unsigned int filter_buffer_sz,
                    float *threshold_buffer, unsigned int threshold_buffer_sz)
{
    p->filter_buffer = filter_buffer;
    p->filter_buffer_sz = filter_buffer_sz;
    p->threshold_buffer = threshold_buffer;
    p->threshold_buffer_sz = threshold_buffer_sz;
    p->state = PEDOMETER_ACC_RISING;
    p->steps = 0;
    p->hysteresis = 0.;
}

void pedometer_process(pedometer_t *p, float sample)
{
    float acc_filt, acc_thr;
    acc_filt = lowpass_process(p->filter_buffer, p->filter_buffer_sz, sample);
    acc_thr = adaptive_threshold_process(p->threshold_buffer,
                                         p->threshold_buffer_sz,
                                         acc_filt);

    if (acc_filt > acc_thr + p->hysteresis && p->state == PEDOMETER_ACC_RISING) {
        p->state = PEDOMETER_ACC_FALLING;
        p->steps += 1;
    } else if (acc_filt < acc_thr - p->hysteresis && p->state == PEDOMETER_ACC_FALLING) {
        p->state = PEDOMETER_ACC_RISING;
    }
}

unsigned int pedometer_get_step_count(pedometer_t *p)
{
    return p->steps;
}
