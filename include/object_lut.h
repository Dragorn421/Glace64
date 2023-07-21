#ifndef OBJECT_LUT_H
#define OBJECT_LUT_H

#include <stdint.h>

#include "include/cglm/cglm.h"
#include "include/cglm/types.h"

// lots of things are going to be reaching into the object_lut, since it also
// contains event handler functions per-object. therefore, it's better to put it
// in it's own compilation medium, to avoid circular inclusion deps.

//// DEFINE EVENT TYPES
// objects should push themselves back on a collision.
typedef struct CollisionEvent { // contains all the data sent to an entity from
                                // the CollisionEventHandler function in the
                                // LUT.

  uint16_t id; // id, based on the index of the objects array.

  vec3 normal; // encode the data of where the object was pushed from, and how
               // far the push should be.
  float magnitude;
} CollisionEvent;

//// DEFINE FUNCTION TYPES
// everything takes in void pointers, need to be very generic about this.
typedef void (*InitFn)(void *); // when it's added to the objects array.
                                // different from construction.
typedef void (*UpdateFn)(void *);
typedef void (*DrawFn)(void *);
typedef void (*CleanFn)(void *); // when it's removed from the objects array.
                                 // different from destruction.
typedef void (*CollisionHandler)(void *, CollisionEvent *);

// have a LUT of ObjectFnPointers for each type of object.
// the functions themselves are passed a void *, and trusted to cast to the
// appropriate type.
typedef struct ObjectFnPointers {
  InitFn init;
  UpdateFn update;
  DrawFn draw;
  CleanFn clean;
  CollisionHandler col_handler;
} ObjectFnPointers;

extern ObjectFnPointers fn_lut[];

#endif // !OBJECT_LUT_H
