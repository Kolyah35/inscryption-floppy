#include "state.h"
#include "pak.h"
#include "graphics.h"

sprite_t* startscreen_title = NULL;
int title_begin_w;
int title_end_x;

void logo_state_init() {
    if (startscreen_title == NULL) {
        startscreen_title = pak_get(&pak_ui, "startscreen_title.png");
    }

    title_begin_w = 8;
    title_end_x = 338;
}

void logo_state_draw() {
    long time = get_state_time_ms(); 

    if (time > 1150) {
        clear_screen(106, 117, 86);
    } else {
        clear_screen(0, 0, 0);
    }

    // i         n
    if (time > 100) {
        draw_sprite_region(startscreen_title, 64, 17, title_begin_w, 46, 60, 72, 0);
        draw_sprite_region(startscreen_title, title_end_x, 17, 362 - title_end_x, 46, 360 - (365 - title_end_x), 72, 0);
    }

    // inscr ption
    if (time > 510) {
        title_begin_w = 133;
        title_end_x = 221;
    } else
    // insc   tion
    if (time > 425) {
        title_begin_w = 99;
        title_end_x = 250;
    } else 
    // ins     ion
    if (time > 340) {
        title_begin_w = 67;
        title_end_x = 283;
    } else
    // in       on
    if (time > 230) {
        title_begin_w = 38;
        title_end_x = 296;
    }

    if (time > 750) {
        float t = (time - 750) / 300.0f;
        if (t > 1.0f) t = 1.0f;

        int letter_y = -40 + (int)(t * 120);  // -40 -> 80
        draw_sprite_region(startscreen_title, 195, 1, 28, 43, 192, letter_y, 0);
    }

    if (time > 1000) {
        float t = (time - 1000) / 70.0f;
        if (t > 1.0f) t = 1.0f;

        int pen_x = -49 + (int)(t * 48);  // -49 -> -1

        draw_sprite_region(startscreen_title, 0, 37, 49, 10, pen_x, 90, 0);
        draw_sprite_region(startscreen_title, 0, 37, 49, 10, FB_WIDTH - 49 - pen_x, 90, SPRITE_FLIP_X);
    }

    if (time > 1150) {
        float t = (time - 1150) / 460.0f;
        if (t > 1.0f) t = 1.0f;

        draw_sprite_region(startscreen_title, 3, 64, 135, 18, 137, 115, 0);

        set_color(255, 239, 172, 255 * (1.f - t));
        fill_rect(0, 0, FB_WIDTH, FB_HEIGHT);
    }
}

State logo_state = {
    logo_state_init,
    logo_state_draw
};