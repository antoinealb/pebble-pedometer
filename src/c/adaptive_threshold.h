#pragma once
#ifndef ADAPTIVE_THRESHOLD_H
#define ADAPTIVE_THRESHOLD_H

#ifdef __cplusplus
extern "C" {
#endif

int adaptive_threshold_process(int *buffer, unsigned long int buffer_sz, int sample);

#ifdef __cplusplus
}
#endif

#endif
