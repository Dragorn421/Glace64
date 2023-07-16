#ifndef INPUT_H
#define INPUT_H

#include <controller.h>

typedef struct SI_condat SI_condat;

// handles updating the global state of the controller once every frame.
// any input calculation in the program should be sourced from the structure
// modified ONLY here. (?)

// you were saying something about multiplayer, so i won't hard-code one player
// into the input structure for now. maybe narrow this data structure if it
// becomes clear we won't do that?

typedef struct PlayerInput {
  SI_condat down; // it bothers me very much that there's redundant data in this
                  // structure, but i won't worry about it for now.
  SI_condat pressed;
  SI_condat up;
} PlayerInput;

typedef struct InputState {
  PlayerInput p1;
} InputState;

extern InputState input_state;

void input_init();
void input_update();

#endif // !INPUT_H
