#ifndef PLAYER_H
#define PLAYER_H

// the player includes the lut header and the lut .c file includes the player
// header. so close to cyclic dep error!!! aghhh!!!!
#include "include/object_lut.h"

#include "include/cglm/types.h"
#include "include/input.h"
#include "include/object.h"
#include "include/physics.h"

typedef struct Player {
  ObjectType type; // every Object "substructure" has to have the same exact
                   // starting structure as the parent. we're casting the player
                   // down to an Object, so it needs to look the same, at least
                   // in the first matching bytes.
  vec3 position;
  uint8_t num_colliders;
  Collider *colliders;

  PlayerInput *input;
  float speed; // units per second
} Player;

Player *player_build(PlayerInput *input);
void player_destroy(Player *p);

void player_init(void *p);
void player_update(void *p);
void player_draw(void *p);
void player_clean(void *p);
void player_handle_collision(void *p, CollisionEvent *e);

#endif // !PLAYER_H
