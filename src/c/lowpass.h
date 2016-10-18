#ifndef LOWPASS_H
#define LOWPASS_H

#ifdef __cplusplus
extern "C" {
#endif

float lowpass_process(float *buffer, unsigned int buffer_sz, float sample);

#ifdef __cplusplus
}
#endif

#endif
