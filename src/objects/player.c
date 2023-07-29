#include "include/objects/player.h"

#include "include/cglm/types.h"
#include "include/cglm/vec3.h"
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
Player *player_build() {
  Player *p = (Player *)malloc(sizeof(Player));
  memcpy(p->position, (vec3){0, -0.9F, 0},
         sizeof(float) * 3); // copy the literal from the stack.
  p->num_colliders = 1;
  p->colliders =
      (Collider *)malloc(sizeof(Collider) * 1); // space for one collider.
  p->colliders[0].type = CL_PILLAR;
  p->type = OBJ_PLAYER;
  p->speed = 0.3F;
  return p;
}

void player_init(void *p) {}

void player_update(void *p) {
  // trust the caller themselves to pass the right type.
  Player *player = (Player *)p;
  float x = input_state.left.pressed.x;
  float y = input_state.left.pressed.y;

  player->position[0] += x * player->speed;
  player->position[2] -= y * player->speed;
}

void player_draw(void *p) { // always draw at the same place, modify position
                            // through matrices in the rendering pipeline.
  Player *player = (Player *)p;
  glprim_cube(player->position);
}

void player_handle_collision(void *p, CollisionEvent *e) {
  Player *player = (Player *)p;
  glm_vec3_scale(e->normalized_force, e->magnitude, e->normalized_force);
  glm_vec3_add(player->position, e->normalized_force, player->position);
}

// maybe don't have a seperate destructor? is there a point to that?
void player_clean(void *p) {
  Player *player = (Player *)p;
  free(player);
}
