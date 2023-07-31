#pragma once

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
