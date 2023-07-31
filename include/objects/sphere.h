#ifndef OBJECT_SPHERE_H
#define OBJECT_SPHERE_H

#include "include/input.h"
#include "include/object.h"

typedef struct ObjectSphere {
  Object o;
  PlayerInput *input;
  float radius;
  int n_segments;
  float rot_pitch;
  float rot_yaw;
} ObjectSphere;

ObjectSphere *objectsphere_build(PlayerInput *input);

void objectsphere_init(void *object);
void objectsphere_update(void *object);
void objectsphere_draw(void *object);
void objectsphere_clean(void *object);

#endif