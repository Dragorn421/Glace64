#ifndef INPUT_H
#define INPUT_H

#include <controller.h>

typedef struct SI_condat SI_condat;

// handles updating the global state of the controller once every frame.
// any input calculation in the program should be sourced from the structure
// modified ONLY here. (?)

// this is redundant and sucks, we'll put up with it for now.
typedef struct PlayerInput {
  SI_condat down;
  SI_condat pressed;
  SI_condat up;
} PlayerInput;

// the input module handles organizing stuff into left and right and global,
// nothing outside of this module should really have a concept of "controller 0
// and controller 1" and etc.
typedef struct InputState {
  PlayerInput left; // for the left and right screens of the splitscreen layout,
                    // respectively.
  PlayerInput right;
  PlayerInput global; // for global inputs, like pausing and etc.
} InputState;

extern InputState input_state;

void input_init(void);
void input_update(void);

#endif // !INPUT_H
