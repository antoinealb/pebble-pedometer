#include <pebble.h>
#include "messages.h"

#define NUM_SAMPLE 100

static Window *main_window;
static Layer *s_canvas_layer;

static int app_running;

static unsigned int step_count = 0;

static void init_clic_callback(void);
static void config_provider(void* context);

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context);
static void draw_rect(GContext *ctx, GRect bounds, int corner_radius, GCornerMask mask);
static void canvas_update_proc(Layer *layer, GContext *ctx);
static void draw_triangle(GContext *ctx, GPoint top, GPoint bottom, GPoint right);
static void draw_pause(GContext *ctx, GPoint top_l, GPoint bottom_l, GPoint top_r, GPoint bottom_r);
static void messages_cb(uint16_t type, AppWorkerMessage *message);


void init_main_window(void)
{
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

    // Draw the 2 rectangles to indicate the functions of the buttons
    GRect top_rect_bounds = GRect(124, 0, 20, 40);
    draw_rect(ctx, top_rect_bounds, corner_radius, GCornersBottom & GCornersLeft);
    graphics_fill_rect(ctx, top_rect_bounds, corner_radius, GCornersBottom & GCornersLeft);

    GRect bottom_rect_bounds = GRect(124, 128, 20, 40);
    draw_rect(ctx, bottom_rect_bounds, corner_radius, GCornersTop & GCornersLeft);

    // Write the initial textstep
    GFont font = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    graphics_context_set_text_color(ctx, GColorBlack);
    char text_top[10];
    snprintf(text_top, sizeof(text_top), "%d", step_count);
    char *text_bottom = "steps";
>>>>>>> Use background worker for accelerometer processing

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

    // Reset button

    int32_t angle_start = DEG_TO_TRIGANGLE(0);
    int32_t angle_end = DEG_TO_TRIGANGLE(300);

    //Create smaller bounds than the container
    GRect reset_symbol_bounds = GRect(bottom_rect_bounds.origin.x+3,
                                    bottom_rect_bounds.origin.y + 2,
                                    bottom_rect_bounds.size.w-6,
                                    bottom_rect_bounds.size.h-4);
    // Draw an arc
    graphics_draw_arc(ctx, reset_symbol_bounds, GOvalScaleModeFitCircle,
                      angle_start, angle_end);


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
}

// event for clic up: start/stop counting steps
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    app_running = !app_running;
}

// event for clic down: reset the counter
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    AppWorkerMessage message;

    // Send the data to the background app
    app_worker_send_message(MESSAGE_STEP_RESET, &message);
}

static void worker_message_cb(uint16_t type, AppWorkerMessage *message)
{
    switch (type) {
        case MESSAGE_STEP_COUNT:
            step_count = message->data0;
            layer_mark_dirty(s_canvas_layer);
            break;

        default:
            break;
    }
}

// deinit function called when the app is closed
void deinit(void)
{
    // Destroy layers and main window
    window_destroy(main_window);
}

int main(void)
{
    app_worker_launch();
    app_worker_message_subscribe(worker_message_cb);

    init_main_window();

    app_event_loop();
    deinit();
}
