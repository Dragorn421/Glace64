#ifndef OBJECT_H
#define OBJECT_H

#include "include/cglm/types.h"
#include "include/physics.h"
#include <stdint.h>

#define NUM_OBJECTS 20

typedef enum ObjectType { OBJ_PLAYER, OBJ_CRAB } ObjectType;

// handled in the external LUT, an object is linked to a FnPointers structure by
// its type.
typedef struct Object {
  ObjectType type;
  vec3 position; // honestly it makes collision easier to assume that each
                 // object has a position so screw it why not

  Collider *colliders; // then, all the collision data for the physics engine to
                       // run through on each object. set this to NULL if the
                       // object has no collision data.
} Object;

typedef struct ObjectState {
  Object *objects[NUM_OBJECTS]; // array of pointers.
} ObjectState;

extern ObjectState object_state;

Object *object_add(Object *o);
void object_remove_by_ptr(Object *o);

void object_init();
void object_update();
void object_draw();
void object_clean();

#endif // !OBJECT_H