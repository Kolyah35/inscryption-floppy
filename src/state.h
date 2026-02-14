#ifndef STATE_H
#define STATE_H
#include <stdint.h>

typedef struct State {
    void(*init)();
    void(*draw)();
} State;

extern State* current_state;
extern uint32_t state_time;

void change_state(State* state);
uint32_t get_state_time_ms();

extern State logo_state;

#endif