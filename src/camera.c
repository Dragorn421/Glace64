#include "include/camera.h"

#include "GL/gl.h"
#include "GL/glu.h"

CameraState camera_state = {.left = {CM_BIRDSEYE}, .right = {CM_BIRDSEYE}};

// handle the generic behavior between each screen state.
static void handle_screen_camera(ScreenCameraState *scs) {
  switch (scs->mode) {
  case CM_BIRDSEYE:
    gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, -1);
    break;
  case CM_FIRSTPERSON:
    break;
  default:
    break;
  }
}

void camera_left() { handle_screen_camera(&camera_state.left); }

void camera_right() {
  glTranslatef(0, 1.0F, 0);
  handle_screen_camera(&camera_state.left);
}
