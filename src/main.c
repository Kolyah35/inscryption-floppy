#include "window.h"
#include "graphics.h"
#include "pak.h"
#include <stdio.h>
#include <stdlib.h>
#include "state.h"

pixel_t* g_framebuffer;

pak_file_t pak_areas;
pak_file_t pak_cards;
pak_file_t pak_map;
pak_file_t pak_objects;
pak_file_t pak_portraits;
pak_file_t pak_sigils;
pak_file_t pak_ui;

#define COUNT_LOADED(pak_load) \
    { int loaded = pak_load; printf("Loaded pak with %i entries\n", loaded); }

int main() {
    if (window_init() != 0) {
        return -1;
    }

    g_framebuffer = malloc(FB_WIDTH * FB_HEIGHT * sizeof(pixel_t));

    COUNT_LOADED(pak_load("resources/areas.pak", &pak_areas));
    COUNT_LOADED(pak_load("resources/cards.pak", &pak_cards));
    COUNT_LOADED(pak_load("resources/map.pak", &pak_map));
    COUNT_LOADED(pak_load("resources/objects.pak", &pak_objects));
    COUNT_LOADED(pak_load("resources/portraits.pak", &pak_portraits));
    COUNT_LOADED(pak_load("resources/sigils.pak", &pak_sigils));
    COUNT_LOADED(pak_load("resources/ui.pak", &pak_ui));

    change_state(&logo_state);

    while (1) {
        if (current_state != NULL) {
            current_state->draw();
        }

        set_color(12, 10, 6, 25);

        for (int i = 0; i < FB_WIDTH; i++) {
            if (i % 2) draw_hline(0, i, FB_WIDTH);
        }
        
        window_draw();
    }

    return 0;
}