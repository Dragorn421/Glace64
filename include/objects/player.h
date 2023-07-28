#ifndef PLAYER_H
#define PLAYER_H

#include "include/input.h"
#include "include/object.h"

typedef struct Player {
  ObjectType type; // every Object "substructure" has to have the same exact
                   // starting structure as the parent. we're casting the player
                   // down to an Object, so it needs to look the same, at least
                   // in the first matching bytes.
  PlayerInput *input;
  float position[3];
  float speed; // units per second
} Player;

Player *player_build(PlayerInput *input);
void player_destroy(Player *p);

void player_init(void *p);
void player_update(void *p);
void player_draw(void *p);
void player_clean(void *p);

#endif // !PLAYER_H
