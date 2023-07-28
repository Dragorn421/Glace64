#include "include/objects/player.h"

#include "include/glprimitives.h"
#include "include/input.h"
#include "include/object.h"
#include "include/physics.h"

#include <GL/gl.h>
#include <libdragon.h>
#include <stdint.h>
#include <stdlib.h>

// use the dummy as the default entity, just for the initial setup of the object
// pipeline in the game.
Player *player_build(PlayerInput *input) {
  Player *p = (Player *)malloc(sizeof(Player));
  // float init_pos[3] = {0, -0.9F, 0};
  // memcpy(p->position, init_pos, sizeof(float) * 3);
  p->type = OBJ_PLAYER;
  p->input = input;
  p->position[0] = 0;
  p->position[1] = -0.9F;
  p->position[2] = 0;
  p->speed = 0.3f;
  return p;
}

void player_init(void *p) {}

void player_update(void *p) {
  // trust the caller themselves to pass the right type.
  Player *player = (Player *)p;
  float x = player->input->pressed.x;
  float y = player->input->pressed.y;

  player->position[0] += x * player->speed * tick_seconds;
  player->position[2] -= y * player->speed * tick_seconds;
}

void player_draw(void *p) { // always draw at the same place, modify position
                            // through matrices in the rendering pipeline.
  Player *player = (Player *)p;
  glprim_cube(player->position);
}

// maybe don't have a seperate destructor? is there a point to that?
void player_clean(void *p) {
  Player *player = (Player *)p;
  free(player);
}
