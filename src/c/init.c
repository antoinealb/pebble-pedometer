#include <pebble.h>
#include "init.h"
#include "pedometer.h"

#define NUM_SAMPLE 100
#define ACTIVE 1

static Window *main_window;
static TextLayer *background_layer;
static TextLayer *step_display_layer;
static TextLayer *hello_display_layer;
static int app_runing;

static pedometer_t meter;
static float filter_buffer[10];
static float threshold_buffer[25];


void init_main_window(void)
{
    memset(filter_buffer, 0, sizeof(filter_buffer));
    memset(threshold_buffer, 0, sizeof(threshold_buffer));
    
    /* Configure pedometer algorithm. */
    pedometer_init(&meter, filter_buffer, sizeof(filter_buffer) / sizeof(float),
                   threshold_buffer, sizeof(threshold_buffer) / sizeof(float));
    meter.hysteresis = 75;


    // Create main Window element and assign to pointer
    main_window = window_create();
    Layer *window_layer = window_get_root_layer(main_window);

    // Create background Layer
    background_layer = text_layer_create(GRect( 0, 0, 144, 168));
    // Setup background layer color (black)
    text_layer_set_background_color(background_layer, GColorClear);


    // Create text Layer to display the steps counter
    step_display_layer = text_layer_create(GRect( 5, 65, 67, 40));
    // Setup layer Information
    text_layer_set_background_color(step_display_layer, GColorBlack);
    text_layer_set_text_color(step_display_layer, GColorWhite);
    text_layer_set_font(step_display_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(step_display_layer, GTextAlignmentCenter);


    // Create text Layer to display a welcome text
    hello_display_layer = text_layer_create(GRect( 20, 0, 100, 20));
    // Setup layer Information
    text_layer_set_background_color(hello_display_layer, GColorClear);
    text_layer_set_text_color(hello_display_layer, GColorWhite);
    text_layer_set_font(hello_display_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(hello_display_layer, GTextAlignmentCenter);

    // Add layers as childs layers to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(background_layer));
    layer_add_child(window_layer, text_layer_get_layer(step_display_layer));
    layer_add_child(window_layer, text_layer_get_layer(hello_display_layer));

    // Show the window on the watch, with animated = true
    window_stack_push(main_window, true);

    init_clic_callback();

    init_accelerometer();
}




/*--------------------------- Clic fonctions --------------------------*/
void init_clic_callback(void)
{
    // Assign specific clic function to the main window
    window_set_click_config_provider(main_window, config_provider);
}

// Assign focus for different type of clics
void config_provider(void* context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

// event for clic up: start counting steps
void up_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    app_runing = 1;
}

// event for clic down: stop counting steps
void down_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    app_runing = 0;
}

// event for clic select: reset the counter
void select_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    meter.steps = 0;
}

/*----------------------------------------------------------------------*/




/*-----------------------Accelerometer fonctions------------------------*/
void init_accelerometer(void)
{
    // Allow accelerometer event
    accel_data_service_subscribe(10, accel_data_handler);
    // Define accelerometer sampling rate
    accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);
}

// Function called every second that get data from the accelerometer
void accel_data_handler(AccelData *data, uint32_t num_samples)
{
    uint32_t n;

    // Send data from y axis to the podometer algorithm
    if (app_runing == ACTIVE) {
        for (n = 0; n < num_samples; n++) {
            pedometer_process(&meter, data[n].z);
        }

        // Print the results on the watch
        int steps = pedometer_get_step_count(&meter);
        static char results[60];
        snprintf(results, sizeof(results), "steps=%d %c", steps, meter.state == PEDOMETER_ACC_RISING ? 'u' : 'd');
        //snprintf(results, 60, "x:%d y:%d z:%d", (int)data[0].x, (int)data[0].y, (int)data[0].z);
        text_layer_set_text(step_display_layer, results);
    }
}
/*------------------------------------------------------------------------*/




// deinit function called when the app is closed
void deinit(void)
{

    // Destroy layers and main window
    text_layer_destroy(background_layer);
    text_layer_destroy(step_display_layer);
    window_destroy(main_window);

    // Stop Accelero
    
    accel_data_service_unsubscribe();
}
