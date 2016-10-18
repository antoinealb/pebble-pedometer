#pragma once
#ifndef INIT_H
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif

void init_main_window(void);

void init_clic_callback(void);
void config_provider(void* context);
void up_single_click_handler(ClickRecognizerRef recognizer, void *context);
void down_single_click_handler(ClickRecognizerRef recognizer, void *context);
void select_single_click_handler(ClickRecognizerRef recognizer, void *context);

void init_accelerometer(void);
void accel_data_handler(AccelData *data, uint32_t num_samples);

void deinit(void);

#ifdef __cplusplus
}
#endif

#endif
