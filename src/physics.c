#include <limits.h>
#include <stdint.h>

#include <libdragon.h>

#include "include/ourmath.h"
#include "include/physics.h"

#include "include/cglm/types.h"
#include "include/object.h"
#include "include/object_lut.h"

PhysicsState physics_state = {}; // dummy for now.

void physics_init(ObjectState *object_state) {}

void physics_update(ObjectState *object_state) { // for now, this just runs
                                                 // through collision, and emits
  // messages to the proper gameobjects.
  int i, j;

  uint8_t is_invalid[NUM_OBJECTS] = {0};

  for (i = 0; i < NUM_OBJECTS; i++) {
    if (is_invalid[i])
      continue;

    Object *base_obj = object_state->objects[i];
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

      Object *target_obj = object_state->objects[i];
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

void physics_clean(ObjectState *object_state) {}

// i don't think these methods are object_state screen-specific? i'm just not
// going to give them an ObjectState* parameter for now.
float tick_seconds;
int tick_microseconds;

static uint32_t next_tick_start_cputicks;

static void physics_set_tick_duration(int tick_duration_us) {
  tick_duration_us =
      CLAMP(tick_duration_us, MIN_TICK_DURATION_US, MAX_TICK_DURATION_US);
  tick_seconds = (float)tick_duration_us / MICROSECONDS_PER_SECOND;
  tick_microseconds = tick_duration_us;
}

void physics_reset_tick(void) {
  next_tick_start_cputicks = TICKS_READ();
  // two justifications for using the minimum as the initial value:
  // - it means initial minimal change from the "previous" (non-existant) tick
  // - it's the same as calling physics_next_tick() immediately after
  //    physics_reset_tick() anyway
  physics_set_tick_duration(MIN_TICK_DURATION_US);
}

void physics_next_tick(void) {
  uint32_t start_cputicks = next_tick_start_cputicks;
  uint32_t end_cputicks = TICKS_READ();

  int32_t elapsed_cputicks = TICKS_DISTANCE(start_cputicks, end_cputicks);
  float seconds_elapsed = (float)elapsed_cputicks / TICKS_PER_SECOND;
  int us_elapsed =
      FLOAT_TO_INT_CHECK(seconds_elapsed * MICROSECONDS_PER_SECOND);

  // Also handles us_elapsed being negative if next_tick_start_cputicks was set
  // in the future
  if (us_elapsed < MIN_TICK_DURATION_US) {
    physics_set_tick_duration(MIN_TICK_DURATION_US);
    // Ensure the real time tracking of ticks corresponds to the physics tick
    // duration (like in the general MIN_TICK_DURATION_US <= us_elapsed <=
    // MAX_TICK_DURATION_US case)
    // Note: this sets next_tick_start_cputicks in the future
    next_tick_start_cputicks = start_cputicks + (uint64_t)MIN_TICK_DURATION_US *
                                                    TICKS_PER_SECOND /
                                                    MICROSECONDS_PER_SECOND;
  } else if (us_elapsed > MAX_TICK_DURATION_US) {
    // Have the physics tick duration be the capped time, but don't use the
    // capped time for tracking real time. Otherwise the game would speed up
    // later to catch up.
    physics_set_tick_duration(MAX_TICK_DURATION_US);
    next_tick_start_cputicks = end_cputicks;
  } else {
    physics_set_tick_duration(us_elapsed);
    // Without the uint64_t cast, the multiplication overflows int=int32_t
    // Note there is some precision loss here on the division so we
    // (lose/gain ?) some time, but it should not be noticeable at all
    next_tick_start_cputicks = start_cputicks + (uint64_t)us_elapsed *
                                                    TICKS_PER_SECOND /
                                                    MICROSECONDS_PER_SECOND;
  }
}
