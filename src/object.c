#include "include/object.h"

#include "include/object_lut.h"
#include "include/objects/player.h"
#include "include/physics.h"

#include <libdragon.h>
#include <signal.h>
#include <stddef.h>

ObjectState object_state = {
    {0}}; // zero alloc the object pointer array to start off with.

// return the pointer as a formality, since the caller will need that to use
// remove_by_ptr on the objects array through this interface.
Object *object_add(Object *o) {
  for (int i = 0; i < NUM_OBJECTS; i++) {
    if (object_state.objects[i] == NULL) {
      // insert into the first NULL slot of the objects array.
      object_state.objects[i] = o;
      fn_lut[o->type].init((void *)o);
      return o;
    }
  }

  debugf("Ran out of Object slots in the objects array!");
  assert(0 == 1); // is there a better way to "panic" and throw a proper error
                  // in libdragon?
  return NULL;
}

// simple remove by pointer, with dumb search algorithm. potentially find a
// better solution for searching and erasing from the list? this seems
// important.
void object_remove_by_ptr(Object *o) {
  for (int i = 0; i < NUM_OBJECTS; i++) {
    if (object_state.objects[i] == o) {
      fn_lut[o->type].clean((void *)o);
      object_state.objects[i] = NULL; // nullify the pointer, so that the search
                                      // algorithms know the slot is free now.
      return;
    }
  }

  debugf("Could not remove object at location %p of type %d, object not found "
         "in the array.",
         o, o->type);
}

// just init the objectstate. calling the init lifecycle method should happen in
// the object_add function.
void object_init() {}

// main lifecycle loops that call every other sub-lifecycle function in each
// object.
void object_update() {
  for (int i = 0; i < NUM_OBJECTS; i++) {
    Object *o = object_state.objects[i];
    if (o != NULL) {
      fn_lut[o->type].update((void *)o);
    }
  }
}

void object_draw() {
  for (int i = 0; i < NUM_OBJECTS; i++) {
    Object *o = object_state.objects[i];
    if (o != NULL) {
      fn_lut[o->type].draw((void *)o);
    }
  }
}

// again, this doesn't call the function, it cleans up the overall ObjectState
// subsystem.
void object_clean() {}
