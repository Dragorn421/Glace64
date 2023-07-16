// SPDX-License-Identifier: CC0-1.0

#include <controller.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <libdragon.h>

#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <GL/glu.h>

#include "include/audio.h"
#include "include/glprimitives.h"
#include "include/splitscreen.h"

void callback(float radius, int n_segments, int iy, int itheta) {
  glColor3f((float)iy / n_segments, (float)itheta / n_segments, 1.0f);
}

int main() {
  debug_init(DEBUG_FEATURE_ALL);
  debugf("hello world!\n");
  controller_init();
  rdpq_init();
  gl_init();
  display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_CORRECT,
               ANTIALIAS_RESAMPLE_FETCH_NEEDED);
  surface_t zbuffer =
      surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());
  float radius = 2.0f;
  int n_segments = 5;
  float rot_pitch = 0.0f;
  float rot_yaw = 0.0f;
  uint32_t ticks_last = TICKS_READ();

  { // setup mats
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, 4.0f / 3.0f, 0.1f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  { // setup filesystem
    dfs_init(DFS_DEFAULT_LOCATION);
  }

  rdpq_font_t *fnt1 = rdpq_font_load("rom:/Roboto-Bold.font64");
  debugf("fnt1 ptr: %p\n", fnt1);

  m_audio_init();
  // m_audio_change_bgm("rom:/AQUA.xm64");

  struct SplitScreenArea area_left = {0, 0, -1, display_get_height()},
                         area_right = {-1, 0, display_get_width(),
                                       display_get_height()};
  float area_left_right_divide_x = display_get_width() * 0.6f;

  while (true) {
    // get delta
    uint32_t ticks_now = TICKS_READ();
    uint32_t ticks_elapsed = TICKS_DISTANCE(ticks_last, ticks_now);
    ticks_last = ticks_now;
    float seconds_elapsed = (float)ticks_elapsed / TICKS_PER_SECOND;

    area_right.bl_x = area_left.tr_x = area_left_right_divide_x;

    // need to do this in the loop, or else we can't show twice on the same
    // display.
    surface_t *disp = display_get();

    rdpq_attach(disp, &zbuffer);
    { // gl loop
      gl_context_begin();

      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      // draw to the left area
      splitscreen_area_activate(&area_left);

      glClearColor(0.2, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
      glRotatef(rot_pitch, 1, 0, 0);
      glRotatef(rot_yaw, 0, 1, 0);

      glprim_sphere(radius, n_segments, callback);

      // draw to the right area
      splitscreen_area_activate(&area_right);

      glClearColor(0, 0.2, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      // reuse the previous transform but with an additional rotation in between
      glMatrixMode(GL_MODELVIEW);
      glRotatef(90, 1, 0, 0);
      glprim_sphere(radius, n_segments, callback);

      // reset the viewport and scissoring (impacts later rdpq_font calls
      // otherwise)
      splitscreen_area_reset();

      gl_context_end();
    }

    { // font loop, the gl loop clears the cfb, so draw text after.
      char fps_str[256];
      sprintf(fps_str, "DELTA: %02f", seconds_elapsed);
      rdpq_font_begin(
          RGBA32(0xED, 0xAE, 0x49, 0xFF)); // gl immediate-mode like syntax for
                                           // drawing font on the screen.
      rdpq_font_position(10, 20);
      rdpq_font_print(fnt1, fps_str);
      rdpq_font_end();
    }
    rdpq_detach_show(); // attach and detach ONCE at the beginning and end of
                        // every rendering pass.

    {
      controller_scan();

      struct controller_data down = get_keys_down();
      struct controller_data pressed = get_keys_pressed();
      float x = pressed.c[0].x / 127.0f / 0.7f;
      float y = pressed.c[0].y / 127.0f / 0.7f;
      float rotspeed_yaw = 360; // degrees per second
      float rotspeed_pitch = 360;

      rot_yaw += x * seconds_elapsed * rotspeed_yaw;
      rot_pitch += y * seconds_elapsed * rotspeed_pitch;

      if (down.c[0].C_up)
        n_segments++;
      if (down.c[0].C_down)
        n_segments--;

      // move the divide between the left and right areas using C left/right
      if (pressed.c[0].C_left || pressed.c[0].C_right)
        area_left_right_divide_x +=
            (pressed.c[0].C_left ? -1 : 1) * 50 * seconds_elapsed;
    }

    m_audio_update();
  }

  m_audio_clean();
}
