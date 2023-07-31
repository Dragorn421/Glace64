#pragma once

typedef enum CameraMode {
  CM_FIRSTPERSON,
  CM_BIRDSEYE,
  CM_COUNT,
} CameraMode;

typedef struct ScreenCameraState {
  CameraMode mode;
} ScreenCameraState;

typedef struct CameraState {
  ScreenCameraState left;
  ScreenCameraState right;
} CameraState;

extern CameraState camera_state;

void camera_left();
void camera_right();
