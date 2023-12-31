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
Player *player_build(PlayerInput *input) {
  Player *p = (Player *)malloc(sizeof(Player));
  memcpy(p->o.position, (vec3){0, -0.9F, 0},
         sizeof(float) * 3); // copy the literal from the stack.
  p->o.num_colliders = 1;
  p->o.colliders =
      (Collider *)malloc(sizeof(Collider) * 1); // space for one collider.
  p->o.colliders[0].type = CL_PILLAR;
  p->o.type = OBJ_PLAYER;
  p->speed = 0.3F;
  p->input = input;
  return p;
}

void player_init(void *p) {}

void player_update(void *p) {
  // trust the caller themselves to pass the right type.
  Player *player = (Player *)p;
  float x = player->input->pressed.x;
  float y = player->input->pressed.y;

  player->o.position[0] += x * player->speed * tick_seconds;
  player->o.position[2] -= y * player->speed * tick_seconds;
}

void player_draw(void *p) {
  Player *player = (Player *)p;
  glprim_cube(
      player->o.position); // the position will be interpreted as a translation
                           // on the modelview matrix before rendering,
                           // therefore offsetting the player properly.
}

void player_handle_collision(void *p, CollisionEvent *e) {
  Player *player = (Player *)p;
  glm_vec3_scale(e->normalized_force, e->magnitude, e->normalized_force);
  glm_vec3_add(player->o.position, e->normalized_force, player->o.position);
}

// maybe don't have a seperate destructor? is there a point to that?
void player_clean(void *p) {
  Player *player = (Player *)p;
  free(player);
}
