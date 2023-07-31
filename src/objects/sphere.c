#include <stdlib.h>

#include <GL/gl.h>

#include "include/glprimitives.h"
#include "include/objects/sphere.h"
#include "include/ourmath.h"
#include "include/physics.h"

ObjectSphere *objectsphere_build(PlayerInput *input) {
  ObjectSphere *os = (ObjectSphere *)calloc(1, sizeof(ObjectSphere));
  os->o.type = OBJ_SPHERE;
  os->input = input;
  return os;
}

void objectsphere_init(void *object) {
  ObjectSphere *os = object;
  os->n_segments = 5;
  os->radius = 2.0f;
  os->rot_pitch = 0;
  os->rot_yaw = 0;
}

void objectsphere_update(void *object) {
  ObjectSphere *os = object;

  if (os->input->down.C_up) {
    os->n_segments += 1;
  }
  if (os->input->down.C_down) {
    os->n_segments = MAX(3, os->n_segments - 1);
  }

  float rotspeed_yaw = 360; // degrees per second (at maximum stick)
  float rotspeed_pitch = 360;

  os->rot_yaw += os->input->pressed.x / 100.0f * tick_seconds * rotspeed_yaw;
  os->rot_pitch +=
      os->input->pressed.y / 100.0f * tick_seconds * rotspeed_pitch;
}

static void callback(float radius, int n_segments, int iy, int itheta) {
  glColor3f((float)iy / n_segments, (float)itheta / n_segments, 1.0f);
}

void objectsphere_draw(void *object) {
  ObjectSphere *os = object;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glRotatef(os->rot_pitch, 1, 0, 0);
  glRotatef(os->rot_yaw, 0, 1, 0);
  glprim_sphere(os->radius, os->n_segments, callback);
  glPopMatrix();
}

void objectsphere_clean(void *object) { free(object); }
