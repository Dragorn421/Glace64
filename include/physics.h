#ifndef PHYSICS_H
#define PHYSICS_H

// for now, just a basic collision engine.

#include <libdragon.h>

typedef enum ColliderType {
  CL_PILLAR, // shapes a pillar around the object. compares the x and z
             // position between a radius.

  CL_SPHERE,  // just checks distance between positions of objects.
  CL_FLOOR,   // if object is below a floor, it's pushed up.
  CL_CEILING, // if object is above a ceiling, it's pushed down.
  CL_COUNT,
} ColliderType;

// every gameobject should have an array of colliders.
typedef struct Collider {
  ColliderType type;
} Collider;

// switch over the type of collider in the physics update.
typedef struct PillarCollider {
  ColliderType type;
  float xz_radius;
} PillarCollider;

typedef struct SphereCollider {
  ColliderType type;
  float radius;
} SphereCollider;

typedef struct CeilingCollider {
  ColliderType type;
  float height;
} CeilingCollider;

typedef struct FloorCollider {
  ColliderType type;
  float height;
} FloorCollider;

typedef struct PhysicsState {
} PhysicsState;

extern PhysicsState physics_state;

void physics_init();
void physics_update();
void physics_clean();

// How much time has passed since the last physics update
// For example at velocity v units/second, an object would move each update at
//  v*tick_seconds
// (these time values are re-computed every frame,
//  they vary with the framerate (lag))
// These values can be seen as "the current physics tick duration"
// Both seconds as float and int as microseconds are available:
//  use int if faster, use float if doing float calculations anyway.
extern float tick_seconds;
extern int tick_microseconds;

#define MICROSECONDS_PER_SECOND 1000000

// The range in which the tick duration is guaranteed to stay (microseconds)
// Note this also means the tick duration will never be 0 or "very small", it is
// safe to divide with it
// min: 60 ticks/s (matching NTSC 60 fps/s, this is reached when the main loop
//  keeps up with the retrace rate)
/*
 * Why 1/60 s and not less:
 * Note that with buffering, the main loop may also temporarily run faster than
 * the retrace rate, hence the need to have a minimum tick duration matching
 * the retrace rate.
 * (for example if there are two framebuffers available and the
 *  iteration takes 1ms, the second frame would be computed with a tick
 *  duration of 1ms, much faster than 60fps, but the two framebuffers will
 *  still be displayed 1/60th of a second apart: the 1ms physics tick will last
 *  1/60th of a second on screen, "distorting" the time. Ensuring tick
 *  durations are at least the retrace period solves this.
 *  Note this can only be an issue over a few frames, as the loop can't run
 *  faster than the VI forever, it needs to wait anyway when eventually no
 *  framebuffer is available)
 */
// TODO I don't think it would make a noticeable difference (see last note
// above about "over a few frames" only) but for PAL builds, use /50
#define MIN_TICK_DURATION_US (MICROSECONDS_PER_SECOND / 60)
// max: 5 ticks/s (if the performance can't keep up the game will update slower
// than it should)
#define MAX_TICK_DURATION_US (MICROSECONDS_PER_SECOND / 5)

/**
 * Call at starting of ticking
 */
void physics_reset_tick(void);

/**
 * Call at the start of each tick
 */
void physics_next_tick(void);

#endif
