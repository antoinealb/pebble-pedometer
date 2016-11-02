#ifndef INIT_INTERN_H
#define INIT_INTERN_H

#ifdef __cplusplus
extern "C" {
#endif
#include <pebble.h>

// Draw functions
static void draw_rect(GContext *ctx, GRect bounds, int corner_radius, GCornerMask mask);
static void canvas_update_proc(Layer *layer, GContext *ctx);
static void draw_triangle(GContext *ctx, GPoint top, GPoint bottom, GPoint right);
static void draw_pause(GContext *ctx, GPoint top_l, GPoint bottom_l, GPoint top_r, GPoint bottom_r);

#ifdef __cplusplus
}
#endif

#endif
