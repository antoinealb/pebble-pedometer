#include <pebble.h>
#include "init.h"
#include "pedometer.h"
#include "init_intern.h"

#define NUM_SAMPLE 100

static Window *main_window;
static Layer *s_canvas_layer;

static int app_running;

static pedometer_t meter;
static float filter_buffer[10];
static float threshold_buffer[25];

static void init_clic_callback(void);
static void config_provider(void* context);
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context);
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context);
static void draw_rect(GContext *ctx, GRect bounds, int corner_radius, GCornerMask mask);
static void canvas_update_proc(Layer *layer, GContext *ctx);
static void draw_triangle(GContext *ctx, GPoint top, GPoint bottom, GPoint right);
static void draw_pause(GContext *ctx, GPoint top_l, GPoint bottom_l, GPoint top_r, GPoint bottom_r);

static void init_accelerometer(void);
static void accel_data_handler(AccelData *data, uint32_t num_samples);

void init_main_window(void)
{
    // The strings for the button layers
    memset(filter_buffer, 0, sizeof(filter_buffer));
    memset(threshold_buffer, 0, sizeof(threshold_buffer));

    /* Configure pedometer algorithm. */
    pedometer_init(&meter, filter_buffer, sizeof(filter_buffer) / sizeof(float),
                   threshold_buffer, sizeof(threshold_buffer) / sizeof(float));
    pedometer_set_hysteresis(&meter, 75);


    app_running = 0;

    // Create main Window element and assign to pointer
    main_window = window_create();
    GRect bounds = layer_get_bounds(window_get_root_layer(main_window));

    // Create canvas layer
    s_canvas_layer = layer_create(bounds);

    // Assign the custom drawing procedure
    layer_set_update_proc(s_canvas_layer, canvas_update_proc);

    // Add to Window
    layer_add_child(window_get_root_layer(main_window), s_canvas_layer);



    // Show the window on the watch, with animated = true
    window_stack_push(main_window, true);

    init_clic_callback();

    init_accelerometer();
}

/*-------------------------- Graphic functions ------------------------*/
// Draw a triangle facing right and fill it
static void draw_triangle(GContext *ctx, GPoint top, GPoint bottom, GPoint right)
{
    // link the 3 points together
    graphics_draw_line(ctx, top, bottom);
    graphics_draw_line(ctx, top, right);
    graphics_draw_line(ctx, bottom, right);
}

static void draw_pause(GContext *ctx, GPoint top_l, GPoint bottom_l, GPoint top_r, GPoint bottom_r)
{
    graphics_draw_line(ctx, top_l, bottom_l);
    graphics_draw_line(ctx, top_r, bottom_r);
}

static void draw_rect(GContext *ctx, GRect bounds, int corner_radius, GCornerMask mask)
{
    graphics_draw_rect(ctx, bounds);
    graphics_fill_rect(ctx, bounds, corner_radius, mask);
}


static void canvas_update_proc(Layer *layer, GContext *ctx)
{
    int corner_radius = 0;

    // Set the line color
    graphics_context_set_stroke_color(ctx, GColorBlack);
    // Set the fill color
    graphics_context_set_fill_color(ctx, GColorBlack);
    GRect bckgrnd_rect = GRect(0, 0, 144, 168);
    draw_rect(ctx, bckgrnd_rect, corner_radius, GCornersAll);



    // Set the line  and fill color for the components
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_fill_color(ctx, GColorWhite);

    // Draw a rectangle for the steps count
    GRect step_rect_bounds = GRect(5, 49, 113, 70);
    graphics_draw_rect(ctx, step_rect_bounds);
    corner_radius = 8;
    graphics_fill_rect(ctx, step_rect_bounds, corner_radius, GCornersAll);

    // Draw the 3 rectangles to indicate the functions of the buttons
    GRect top_rect_bounds = GRect(124, 0, 20, 40);
    draw_rect(ctx, top_rect_bounds, corner_radius, GCornersBottom & GCornersLeft);
    graphics_fill_rect(ctx, top_rect_bounds, corner_radius, GCornersBottom & GCornersLeft);

    GRect mid_rect_bounds = GRect(124, 61, 20, 46);
    draw_rect(ctx, mid_rect_bounds, corner_radius, GCornersLeft);

    GRect bottom_rect_bounds = GRect(124, 128, 20, 40);
    draw_rect(ctx, bottom_rect_bounds, corner_radius, GCornersTop & GCornersLeft);

    // Write the initial textstep
    GFont font = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    graphics_context_set_text_color(ctx, GColorBlack);
    char text_top[10];
    snprintf(text_top, sizeof(text_top), "%d", pedometer_get_step_count(&meter));
    char *text_bottom = "steps";

    // Determine a reduced bounding box
    GRect txt_top_bounds = GRect(step_rect_bounds.origin.x,
                                 step_rect_bounds.origin.y,
                                 step_rect_bounds.size.w,
                                 step_rect_bounds.size.h / 2);
    GRect txt_bottom_bounds = GRect(step_rect_bounds.origin.x,
                                    step_rect_bounds.origin.y + step_rect_bounds.size.h / 2 - 5,
                                    step_rect_bounds.size.w,
                                    step_rect_bounds.size.h / 2);

    // Draw the text
    graphics_draw_text(ctx, text_top, font, txt_top_bounds, GTextOverflowModeWordWrap,
                       GTextAlignmentCenter, NULL);
    graphics_draw_text(ctx, text_bottom, font, txt_bottom_bounds, GTextOverflowModeWordWrap,
                       GTextAlignmentCenter, NULL);

    graphics_context_set_stroke_width(ctx, 5);
    // Draw the info about what action each button perform

    // Play-Pause button
    GPoint top = GPoint(130, 6);
    GPoint bottom = GPoint(130, 14);
    GPoint right = GPoint(138, 10);
    draw_triangle(ctx, top, bottom, right);

    GPoint top_l = GPoint(130, 22);
    GPoint bottom_l = GPoint(130, 30);
    GPoint top_r = GPoint(138, 22);
    GPoint bottom_r = GPoint(138, 30);
    draw_pause(ctx, top_l, bottom_l, top_r, bottom_r);

    // See history button
    font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
    char *text = "Logs";
    // Determine a reduced bounding box
    GRect txt_bounds = GRect(mid_rect_bounds.origin.x,
                             mid_rect_bounds.origin.y,
                             mid_rect_bounds.size.w,
                             mid_rect_bounds.size.h);
    // Draw the text
    graphics_draw_text(ctx,
                       text,
                       font,
                       txt_bounds,
                       GTextOverflowModeWordWrap,
                       GTextAlignmentCenter,
                       NULL);

    // Store button
    text = "Store";
    // Determine a reduced bounding box
    txt_bounds = GRect(bottom_rect_bounds.origin.x,
                       bottom_rect_bounds.origin.y,
                       bottom_rect_bounds.size.w,
                       bottom_rect_bounds.size.h);
    // Draw the text
    graphics_draw_text(ctx,
                       text,
                       font,
                       txt_bounds,
                       GTextOverflowModeWordWrap,
                       GTextAlignmentCenter,
                       NULL);

}

/*--------------------------- Clic fonctions --------------------------*/
static void init_clic_callback(void)
{
    // Assign specific clic function to the main window
    window_set_click_config_provider(main_window, config_provider);
}

// Assign focus for different type of clics
static void config_provider(void* context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

// event for clic up: start counting steps
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    app_running = !app_running;
}

// event for clic down: stop counting steps
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    app_running = 0;
}

// event for clic select: reset the counter
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    pedometer_reset_step_count(&meter);
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
    if (app_running) {
        for (n = 0; n < num_samples; n++) {
            pedometer_process(&meter, data[n].z);
        }

        // Print the results on the watch
        layer_mark_dirty(s_canvas_layer);
    }
}
/*------------------------------------------------------------------------*/

// deinit function called when the app is closed
void deinit(void)
{

    // Destroy layers and main window
    window_destroy(main_window);

    // Stop Accelerometer
    accel_data_service_unsubscribe();
}

int main(void)
{

    init_main_window();

    app_event_loop();
    deinit();
}
