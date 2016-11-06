#include <pebble.h>
#include "messages.h"

#define NUM_SAMPLE 100

static Window *main_window;
static Layer *s_canvas_layer;

static unsigned int step_count = 0;
static unsigned int app_running = true;

static GBitmap *play_bitmap;
static GBitmap *pause_bitmap;
static GBitmap *reset_bitmap;
static BitmapLayer *reset_bitmap_layer;
static BitmapLayer *play_bitmap_layer;
static BitmapLayer *pause_bitmap_layer;

static void button_config_provider(void* context);
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context);
static void draw_rect(GContext *ctx, GRect bounds, int corner_radius, GCornerMask mask);
static void canvas_update_proc(Layer *layer, GContext *ctx);
static void worker_message_cb(uint16_t type, AppWorkerMessage *message);

void init_main_window(void)
{
    // Create main Window element and assign to pointer
    main_window = window_create();
    GRect bounds = layer_get_bounds(window_get_root_layer(main_window));

    // Create canvas layer
    s_canvas_layer = layer_create(bounds);

    // Assign the custom drawing procedure
    layer_set_update_proc(s_canvas_layer, canvas_update_proc);

    // Add to Window
    layer_add_child(window_get_root_layer(main_window), s_canvas_layer);

    // bitmap for reset arrows
    reset_bitmap = gbitmap_create_with_resource(RESOURCE_ID_RESET_ARROWS);

    reset_bitmap_layer = bitmap_layer_create(GRect(123, 135, 20, 20));
    bitmap_layer_set_compositing_mode(reset_bitmap_layer, GCompOpSet);
    bitmap_layer_set_bitmap(reset_bitmap_layer, reset_bitmap);

    layer_add_child(window_get_root_layer(main_window),
                    bitmap_layer_get_layer(reset_bitmap_layer));

    // Bitmap for play
    play_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PLAY_BUTTON);

    play_bitmap_layer = bitmap_layer_create(GRect(123, 2, 20, 20));
    bitmap_layer_set_compositing_mode(play_bitmap_layer, GCompOpSet);
    bitmap_layer_set_bitmap(play_bitmap_layer, play_bitmap);


    // Bitmap for pause
    pause_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PAUSE_BUTTON);

    pause_bitmap_layer = bitmap_layer_create(GRect(123, 24, 20, 20));
    bitmap_layer_set_compositing_mode(pause_bitmap_layer, GCompOpSet);
    bitmap_layer_set_bitmap(pause_bitmap_layer, pause_bitmap);


    // Show the window on the watch, with animated = true
    window_stack_push(main_window, true);

    // Assign specific clic function to the main window
    window_set_click_config_provider(main_window, button_config_provider);
}

static void draw_rect(GContext *ctx, GRect bounds, int corner_radius, GCornerMask mask)
{
    graphics_draw_rect(ctx, bounds);
    graphics_fill_rect(ctx, bounds, corner_radius, mask);
}


static void canvas_update_proc(Layer *layer, GContext *ctx)
{
    int corner_radius = 0;


    if (app_running) {
        layer_add_child(layer,
                        bitmap_layer_get_layer(pause_bitmap_layer));
    } else {
        layer_add_child(layer,
                        bitmap_layer_get_layer(play_bitmap_layer));
    }

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
    GRect top_rect_bounds = GRect(122, 0, 22, 48);
    draw_rect(ctx, top_rect_bounds, corner_radius, GCornersBottom & GCornersLeft);
    graphics_fill_rect(ctx, top_rect_bounds, corner_radius, GCornersBottom & GCornersLeft);

    GRect bottom_rect_bounds = GRect(122, 128, 22, 40);
    draw_rect(ctx, bottom_rect_bounds, corner_radius, GCornersTop & GCornersLeft);

    // Write the initial textstep
    GFont font = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    graphics_context_set_text_color(ctx, GColorBlack);
    char text_top[10];
    snprintf(text_top, sizeof(text_top), "%d", step_count);
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

}

// Assign focus for different type of clics
static void button_config_provider(void* context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

// event for clic up: start/stop counting steps
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    AppWorkerMessage message;

    // Send the data to the background app
    app_worker_send_message(MESSAGE_PLAY_PAUSE, &message);

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
            app_running = message->data1;
            layer_remove_child_layers(s_canvas_layer);
            break;

        default:
            break;
    }
}

// deinit function called when the app is closed
void deinit(void)
{
    // destroy the bitmaps
    gbitmap_destroy(pause_bitmap);
    bitmap_layer_destroy(pause_bitmap_layer);

    gbitmap_destroy(reset_bitmap);
    bitmap_layer_destroy(reset_bitmap_layer);

    gbitmap_destroy(play_bitmap);
    bitmap_layer_destroy(play_bitmap_layer);

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
