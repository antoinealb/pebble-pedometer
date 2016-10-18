#ifndef ADAPTIVE_THRESHOLD_H
#define ADAPTIVE_THRESHOLD_H

#ifdef __cplusplus
extern "C" {
#endif

float adaptive_threshold_process(float *buffer, unsigned int buffer_sz, float sample);

#ifdef __cplusplus
}
#endif

#endif
