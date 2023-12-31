#ifndef OBJECT_H
#define OBJECT_H

#include "include/cglm/types.h"
#include "include/physics_colliders.h"
#include <stdint.h>

#define NUM_OBJECTS 20

typedef enum ObjectType {
  OBJ_PLAYER,
  OBJ_CUBE,
  OBJ_FLOOR,
  OBJ_CRAB,
  OBJ_SPHERE
} ObjectType;

// handled in the external LUT, an object is linked to a FnPointers structure by
// its type.
typedef struct Object {
  ObjectType type;
  vec3 position; // honestly it makes collision easier to assume that each
                 // object has a position so screw it why not

  uint8_t num_colliders;
  Collider *colliders; // then, all the collision data for the physics engine to
                       // run through on each object. set this to NULL if the
                       // object has no collision data.
} Object;

typedef struct ObjectState {
  Object *objects[NUM_OBJECTS]; // array of pointers.
} ObjectState;

extern ObjectState left_objects;
extern ObjectState right_objects;

Object *object_add(ObjectState *object_state, Object *o);
void object_remove_by_ptr(ObjectState *object_state, Object *o);

void object_init(ObjectState *object_state);
void object_update(ObjectState *object_state);
void object_draw(ObjectState *object_state);
void object_clean(ObjectState *object_state);

#endif // !OBJECT_H
