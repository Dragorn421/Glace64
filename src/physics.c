#include "include/physics.h"
#include "include/cglm/types.h"
#include "include/object.h"
#include "include/object_lut.h"
#include <stdint.h>

PhysicsState physics_state = {}; // dummy for now.

void physics_init() {}

void physics_update() { // for now, this just runs through collision, and emits
                        // messages to the proper gameobjects.
  int i, j;

  uint8_t is_invalid[NUM_OBJECTS] = {0};

  for (i = 0; i < NUM_OBJECTS; i++) {
    if (is_invalid[i])
      continue;

    Object *base_obj = object_state.objects[i];
    if (base_obj == NULL) {
      is_invalid[i] = 1;
      continue;
    }

    Collider *base_colliders = base_obj->colliders;
    if (base_colliders == NULL) // if the colliders array is garbage, skip
    {
      is_invalid[i] = 1;
      continue;
    }

    for (j = 0; j < NUM_OBJECTS; j++) {
      if (is_invalid[j])
        continue;

      if (i == j)
        continue;

      Object *target_obj = object_state.objects[i];
      if (target_obj == NULL)
        continue;

      Collider *target_colliders = target_obj->colliders;
      if (target_colliders == NULL) // if the colliders array is garbage, skip
        continue;

      int col_i, col_j;

      // both i and j objects are valid collision objects, and different
      // ones, so check the force and generate an event from i -> j.
      CollisionEvent *e = (CollisionEvent *)calloc(sizeof(CollisionEvent), 1);
      // else, the object is valid and has a valid collision array.

      memcpy(e->normalized_force, (vec3){0, 0, 0}, sizeof(vec3));
      e->magnitude = 0.00F; // empty force, doesn't move anything.

      e->id = i; // use the entity id as the id for collisions.

      // apply the forces to the collisionevent, compounding them into one big
      // response.
      for (col_i = 0; col_i < base_obj->num_colliders; col_i++) {
        Collider base_collider = base_obj->colliders[col_i];

        switch (base_collider.type) {
        case CL_FLOOR: {
          if (base_obj->position[1] >
              target_obj->position[1]) { // if the floor is above the target,
                                         // then push it up.
            memcpy(e->normalized_force, (vec3){0, 1, 0}, sizeof(float) * 3);
            e->magnitude += 1;
          }
        } break;
        case CL_PILLAR: {
          for (col_j = 0; col_j < target_obj->num_colliders; col_j++) {
            // Collider target_collider = target_obj->colliders[col_j];
          }
        } break;
        default:
          break;
        }
      }

      // then, send the batched collision response over the function pointer to
      // the target object, allowing them to react however they will.
      fn_lut[target_obj->type].col_handler(
          (void *)target_obj,
          e); // pass in the collisionevent DIRECTLY.
    }
  }
}

void physics_clean() {}
