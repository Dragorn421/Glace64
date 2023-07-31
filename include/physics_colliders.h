#pragma once

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
