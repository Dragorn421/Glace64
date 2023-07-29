#include "include/input.h"

InputState input_state = {0}; // just zero everything out initially.

void input_init(void) { controller_init(); }

// is it faster to static alloc these, rather than stack allocating them in the
// input function itself?
static struct controller_data down;
static struct controller_data pressed;
static struct controller_data up;

void input_update(void) {
  controller_scan();

  // get the LOCAL controller_data state
  down = get_keys_down();
  pressed = get_keys_pressed();
  up = get_keys_up();

  // left mapped directly to the first player for now.
  input_state.left.down = down.c[0];
  input_state.left.pressed = pressed.c[0];
  input_state.left.up = up.c[0];
}
