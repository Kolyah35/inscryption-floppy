#include "graphics.h"

int g_color = 0xFFFFFFFF;

void set_color_int(int new_color) {
    g_color = new_color;
}

void set_color(uint8_t r, uint8_t g, uint8_t b) {
    g_color = (r << 16) | (g << 8) | b;
}

static inline void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    g_framebuffer[y * FB_WIDTH + x] = (r << 16) | (g << 8) | b;
}

static inline void draw_pixel_int(int x, int y, int color) {
    g_framebuffer[y * FB_WIDTH + x] = color;
}

static inline uint8_t blend_channel(uint8_t src, uint8_t dst, uint8_t a) {
    return (src * a + dst * (255 - a)) >> 8;
}

void blend_pixel(uint32_t* dst_pixel, uint8_t* src_rgba) {
    uint8_t src_r = src_rgba[0];
    uint8_t src_g = src_rgba[1];
    uint8_t src_b = src_rgba[2];
    uint8_t a     = src_rgba[3];

    if (a == 255) {
        *dst_pixel = (src_r << 16) | (src_g << 8) | src_b;
        return;
    }

    if (a == 0)
        return;

    uint8_t dst_r = (*dst_pixel >> 16) & 0xFF;
    uint8_t dst_g = (*dst_pixel >> 8)  & 0xFF;
    uint8_t dst_b = (*dst_pixel)       & 0xFF;

    uint8_t out_r = blend_channel(src_r, dst_r, a);
    uint8_t out_g = blend_channel(src_g, dst_g, a);
    uint8_t out_b = blend_channel(src_b, dst_b, a);

    *dst_pixel = (out_r << 16) | (out_g << 8) | out_b;
}

void fill_rect(int x, int y, int w, int h) {
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w >= FB_WIDTH) { w = FB_WIDTH - x; }
    if (x + h >= FB_WIDTH) { h = FB_HEIGHT - y; }

    while (w--) {
        while (h--) {
            draw_pixel_int(x + w, y + h, g_color);
        }
    }
}

void clear_screen(uint8_t r, uint8_t g, uint8_t b) {
    pixel_t col = (r << 16) | (g << 8) | b;

    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) g_framebuffer[i] = col;
}

void draw_sprite_region(sprite_t* texture, int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y) {
    if (dest_x < 0) { 
        src_x += -dest_x;
        src_w -= -dest_x;
        dest_x = 0; 
    }
    
    if (dest_y < 0) { 
        src_y += -dest_y;
        src_h -= -dest_y;
        dest_y = 0; 
    } 
    
    if (dest_x + src_w >= FB_WIDTH) src_w = FB_WIDTH - dest_x;
    if (dest_y + src_h >= FB_HEIGHT) src_h = FB_HEIGHT - dest_y;

    for (int x = src_x; x < src_x + src_w; x++) {
        for (int y = src_y; y < src_y + src_h; y++) {
            int i = y * texture->width + x;
            uint8_t r = texture->rgba[i * 4 + 0];
            uint8_t g = texture->rgba[i * 4 + 1];
            uint8_t b = texture->rgba[i * 4 + 2];

            int out_x = dest_x + x - src_x;
            int out_y = dest_y + y - src_y;

            uint32_t* dst_pixel = &g_framebuffer[out_y * FB_WIDTH + out_x];
            uint8_t* src_pixel = &texture->rgba[(y * texture->width + x) * 4];

            blend_pixel(dst_pixel, src_pixel);
        }
    }
}

void draw_sprite(sprite_t* texture, int dest_x, int dest_y) {
    draw_sprite_region(texture, 0, 0, texture->width, texture->height, dest_x, dest_y);
}
