#ifndef CUBE_H
#define CUBE_H

#include "include/object_lut.h"

#include "include/cglm/types.h"
#include "include/object.h"
#include "include/physics.h"

typedef struct Cube {
  ObjectType type;
  vec3 position;
  uint8_t num_colliders;
  Collider *colliders;

  float speed;
} Cube;

Cube *cube_build(vec3 position);
void cube_destroy(Cube *c);

void cube_init(void *c);
void cube_update(void *c);
void cube_draw(void *c);
void cube_clean(void *c);
void cube_handle_collision(void *c, CollisionEvent *e);

#endif // !CUBE_H
