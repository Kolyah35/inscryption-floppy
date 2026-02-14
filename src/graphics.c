#include "graphics.h"

static uint8_t g_r = 255;
static uint8_t g_g = 255;
static uint8_t g_b = 255;
static uint8_t g_a = 255;

void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    g_r = r;
    g_g = g;
    g_b = b;
    g_a = a;
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
    if (w <= 0 || h <= 0)
        return;

    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }

    if (x + w > FB_WIDTH)
        w = FB_WIDTH - x;

    if (y + h > FB_HEIGHT)
        h = FB_HEIGHT - y;

    if (w <= 0 || h <= 0)
        return;

    if (g_a == 255) {
        uint32_t color = (g_r << 16) | (g_g << 8) | g_b;

        for (int iy = 0; iy < h; iy++) {
            uint32_t* row = g_framebuffer + (y + iy) * FB_WIDTH + x;
            for (int ix = 0; ix < w; ix++)
                row[ix] = color;
        }
        return;
    }

    if (g_a == 0)
        return;

    for (int iy = 0; iy < h; iy++) {
        uint32_t* row = g_framebuffer + (y + iy) * FB_WIDTH + x;

        for (int ix = 0; ix < w; ix++) {
            uint32_t dst = row[ix];

            uint8_t dst_r = (dst >> 16) & 0xFF;
            uint8_t dst_g = (dst >> 8)  & 0xFF;
            uint8_t dst_b =  dst        & 0xFF;

            uint8_t out_r = blend_channel(g_r, dst_r, g_a);
            uint8_t out_g = blend_channel(g_g, dst_g, g_a);
            uint8_t out_b = blend_channel(g_b, dst_b, g_a);

            row[ix] = (out_r << 16) | (out_g << 8) | out_b;
        }
    }
}

void clear_screen(uint8_t r, uint8_t g, uint8_t b) {
    pixel_t col = (r << 16) | (g << 8) | b;

    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) g_framebuffer[i] = col;
}

void draw_sprite_region(sprite_t* texture, int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y, int flags) {
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

    if (dest_x + src_w > FB_WIDTH)
        src_w = FB_WIDTH - dest_x;

    if (dest_y + src_h > FB_HEIGHT)
        src_h = FB_HEIGHT - dest_y;

    if (src_w <= 0 || src_h <= 0)
        return;

    int flip_x = flags & SPRITE_FLIP_X;
    int flip_y = flags & SPRITE_FLIP_Y;

    int start_x = (flip_x)? (src_x + src_w - 1) : src_x;
    int start_y = (flip_y)? (src_y + src_h - 1) : src_y;

    int step_x  = (flip_x)? -1 : 1;
    int step_y  = (flip_y)? -1 : 1;

    for (int y = 0; y < src_h; y++) {
        int sample_y = start_y + y * step_y;

        for (int x = 0; x < src_w; x++) {
            int sample_x = start_x + x * step_x;
            int out_x = dest_x + x;
            int out_y = dest_y + y;

            uint8_t* src_pixel = &texture->rgba[(sample_y * texture->width + sample_x) * 4];
            uint32_t* dst_pixel = &g_framebuffer[out_y * FB_WIDTH + out_x];

            blend_pixel(dst_pixel, src_pixel);
        }
    }
}

void draw_sprite(sprite_t* texture, int dest_x, int dest_y, int flags) {
    draw_sprite_region(texture, 0, 0, texture->width, texture->height, dest_x, dest_y, flags);
}

void draw_hline(int x, int y, int width) {
    if (y < 0 || y >= FB_HEIGHT)
        return;

    if (x < 0) {
        width += x;
        x = 0;
    }

    if (x + width > FB_WIDTH)
        width = FB_WIDTH - x;

    if (width <= 0)
        return;

    if (g_a == 255) {
        while(--width) draw_pixel(x + width, y, g_r, g_g, g_b);
        return;
    }

    if (g_a == 0)
        return;

    while(--width) {
        uint32_t dst = g_framebuffer[y * FB_WIDTH + x + width];

        uint8_t dst_r = (dst >> 16) & 0xFF;
        uint8_t dst_g = (dst >> 8)  & 0xFF;
        uint8_t dst_b =  dst        & 0xFF;

        uint8_t out_r = blend_channel(g_r, dst_r, g_a);
        uint8_t out_g = blend_channel(g_g, dst_g, g_a);
        uint8_t out_b = blend_channel(g_b, dst_b, g_a);

        draw_pixel(x + width, y, out_r, out_g, out_b);
    }
}