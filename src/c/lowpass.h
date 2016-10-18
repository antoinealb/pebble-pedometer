#pragma once
#ifndef LOWPASS_H
#define LOWPASS_H

#ifdef __cplusplus
extern "C" {
#endif

int lowpass_process(int *buffer, unsigned long int buffer_sz, int sample);

#ifdef __cplusplus
}
#endif

#endif
