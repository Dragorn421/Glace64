#include "include/physics.h"
#include "include/object.h"
#include "include/object_lut.h"

PhysicsState physics_state = {}; // dummy for now.

void physics_init() {}

void physics_update() { // for now, this just runs through collision, and emits
                        // messages to the proper gameobjects.
  int i;

  for (i = 0; i < NUM_OBJECTS; i++) {
    Object *o = object_state.objects[i];
    if (o == NULL)
      continue;

    Collider *cols = o->colliders;
    if (cols == NULL) // if the colliders array is garbage, skip
      continue;

    CollisionEvent *e = (CollisionEvent *)calloc(sizeof(CollisionEvent), 1);
    // else, the object is valid and has a valid collision array.

    e->id = i; // literally just set the id to the index. the index into the
               // objects array behaves EXACTLY like an entity id in an
               // ecs-style design. it's unique to the object until that object
               // gets removed, so it can be relied on.

    memcpy(e->normal, (vec3){1.0F, 0, 0}, sizeof(vec3));
    e->magnitude = 0.00F; // empty force, doesn't move anything.

    // call the function directly, just like in objects.c
    fn_lut[o->type].col_handler((void *)o,
                                e); // pass in the collisionevent DIRECTLY.
  }
}

void physics_clean() {}
