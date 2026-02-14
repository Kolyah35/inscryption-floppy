#ifndef GRAPHICS_H
#define GRAPHICS_H

#define FB_WIDTH  420
#define FB_HEIGHT 240

#include <stdint.h>
#include "pak.h"

typedef uint32_t pixel_t;

extern pixel_t* g_framebuffer;

void clear_screen(uint8_t r, uint8_t g, uint8_t b);

void draw_sprite_region(sprite_t* texture, int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y);
void draw_sprite(sprite_t* texture, int x, int y);

void set_color_int(int color);
void set_color(uint8_t r, uint8_t g, uint8_t b);

void fill_rect(int x, int y, int w, int h);
#endif