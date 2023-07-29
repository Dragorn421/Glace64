#ifndef FLOOR_H
#define FLOOR_H

#include "include/object_lut.h"

#include "include/cglm/types.h"
#include "include/object.h"
#include "include/physics.h"

typedef struct Floor {
  ObjectType type;
  vec3 position;
  uint8_t num_colliders;
  Collider *colliders;
} Floor;

Floor *floor_build(float y_pos);
void floor_destroy(Floor *f);

void floor_init(void *f);
void floor_update(void *f);
void floor_draw(void *f);
void floor_clean(void *f);
void floor_handle_collision(void *f, CollisionEvent *e);

#endif // !FLOOR_H
