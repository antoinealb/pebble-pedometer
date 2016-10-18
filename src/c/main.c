/*---------------------------------------------------------------------------
SmartWatch Group 6, "System Engineering" 2016, EPFL

Authors: Kaufmann Florian, Benvenuti Eloi, Albertelli Antoine
Version: 1.0
Date: 13.10.2016

---------------------------------------------------------------------------*/

#include <pebble.h>
#include <stdio.h>
#include "pedometer.h"
#include "init.h"

int main(void) {
          
    init_main_window();
  
    /* Configure pedometer algorithm. */
    meter.hysteresis = 100;
    int filter_buffer[20];
    int threshold_buffer[50];
    pedometer_init(&meter, filter_buffer, sizeof(filter_buffer)/sizeof(int),
                   threshold_buffer, sizeof(threshold_buffer)/sizeof(int));
  
    app_event_loop();
    deinit();
}