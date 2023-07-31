#include <string.h>

#include "include/input.h"

InputState input_state = {0}; // just zero everything out initially.

struct {
  bool redirect[4][CONTROLPART_NUM];
} input_redirections = {0};

void input_init(void) { controller_init(); }

/**
 * Set if controller controller_index drives control_part
 * @param controller_index 0, 1, 2 or 3
 * @param control_part left, right or global, see enum ControlPart
 * @param redirect whether to redirect
 */
void input_set_redirect(int controller_index, enum ControlPart control_part,
                        bool redirect) {
  input_redirections.redirect[controller_index][control_part] = redirect;
}

void input_clear_redirects(void) {
  memset(&input_redirections, 0, sizeof(input_redirections));
}

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

  PlayerInput *playerInput_by_controlPart[] = {
      [CONTROLPART_LEFT] = &input_state.left,
      [CONTROLPART_RIGHT] = &input_state.right,
      [CONTROLPART_GLOBAL] = &input_state.global,
  };

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < CONTROLPART_NUM; j++) {
      if (input_redirections.redirect[i][j]) {
        playerInput_by_controlPart[j]->down = down.c[i];
        playerInput_by_controlPart[j]->pressed = pressed.c[i];
        playerInput_by_controlPart[j]->up = up.c[i];
      }
    }
  }
}
