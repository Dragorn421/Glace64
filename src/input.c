#include "include/input.h"

InputState input_state = {0}; // just zero everything out initially.

void input_init() { controller_init(); }

// is it faster to static alloc these, rather than stack allocating them in the
// input function itself?
static struct controller_data down;
static struct controller_data pressed;
static struct controller_data up;

void input_update() {
  controller_scan();

  // get the LOCAL controller_data state
  down = get_keys_down();
  pressed = get_keys_pressed();
  up = get_keys_up();

  input_state.p1.down = down.c[0];
  input_state.p1.pressed = pressed.c[0];
  input_state.p1.up = up.c[0];

  // then preprocess and assign to the input structure.
  input_state.p1.pressed.x = input_state.p1.pressed.x / 127.0f / 0.7f;
  input_state.p1.pressed.y = input_state.p1.pressed.y / 127.0f / 0.7f;
}
