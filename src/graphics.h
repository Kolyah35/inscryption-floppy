#ifndef GRAPHICS_H
#define GRAPHICS_H

#define FB_WIDTH  420
#define FB_HEIGHT 240

#include <stdint.h>
#include "pak.h"

#define SPRITE_FLIP_X  (1 << 0)
#define SPRITE_FLIP_Y  (1 << 1)

typedef uint32_t pixel_t;

extern pixel_t* g_framebuffer;

void clear_screen(uint8_t r, uint8_t g, uint8_t b);

void draw_sprite_region(sprite_t* texture, int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y, int flags);
void draw_sprite(sprite_t* texture, int x, int y, int flags);

void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void fill_rect(int x, int y, int w, int h);
void draw_hline(int x, int y, int width);
#endif