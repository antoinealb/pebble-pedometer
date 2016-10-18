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

    app_event_loop();
    deinit();
}
