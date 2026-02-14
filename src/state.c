#include "state.h"
#include "window.h"

State* current_state = NULL;
uint32_t state_time;

void change_state(State* state) {
    if (state->init) state->init();
    current_state = state;
    state_time = window_time();
}

uint32_t get_state_time_ms() {
    return window_time() - state_time;
}