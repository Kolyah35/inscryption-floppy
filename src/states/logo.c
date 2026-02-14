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
    clear_screen(0, 0, 0);

    long time = get_state_time_ms(); 

    // i         n
    if (time > 100) {
        draw_sprite_region(startscreen_title, 64, 17, title_begin_w, 46, 60, 72);
        draw_sprite_region(startscreen_title, title_end_x, 17, 362 - title_end_x, 46, 360 - (365 - title_end_x), 72);
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
        draw_sprite_region(startscreen_title, 195, 1, 28, 43, 192, letter_y);
    }


}

State logo_state = {
    logo_state_init,
    logo_state_draw
};