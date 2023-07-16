#include <libdragon.h>

#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <GL/glu.h>

#include "include/splitscreen.h"

void splitscreen_area_activate(struct SplitScreenArea *area) {
  int x = area->bl_x, y = area->bl_y;
  int w = area->tr_x - area->bl_x, h = area->tr_y - area->bl_y;

  // Enable scissoring: only pixels in this region will be drawn to
  glEnable(GL_SCISSOR_TEST);
  glScissor(x, y, w, h);

  // Set the viewport (for the transform from -1,1 coordinates to window
  // coordinates: instead of mapping to the full window, map to the target area)
  glViewport(x, y, w, h);

  // make a projection matrix matching the area
  // ideally left to the caller? eventually this would clash with a camera
  // system for now this is mainly to fix the aspect ratio
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(80, (float)w / h, 0.1, 30);
}

void splitscreen_area_reset(void) {
  glDisable(GL_SCISSOR_TEST);
  glViewport(0, 0, display_get_width(), display_get_height());
}
