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

#include "display.h"
#include "include/audio.h"
#include "include/cglm/types.h"
#include "include/etc_math.h"
#include "include/glprimitives.h"
#include "include/input.h"
#include "include/object.h"
#include "include/objects/cube.h"
#include "include/objects/floor.h"
#include "include/physics.h"
#include "include/splitscreen.h"

// object type includes
#include "include/audio/mp3.h"
#include "include/audio/simple_sinewave.h"
#include "include/objects/player.h"
#include "mixer.h"

int main() {
  debug_init(DEBUG_FEATURE_ALL);
  debugf("hello world!\n");

  m_audio_init();

  rdpq_init();

  input_init();

  gl_init();
  display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_CORRECT,
               ANTIALIAS_RESAMPLE_FETCH_NEEDED);
  surface_t zbuffer =
      surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

  uint32_t ticks_last = TICKS_READ();

  object_init();
  Object *player_obj = object_add((Object *)player_build());
  object_add((Object *)cube_build((vec3){0, -0.9F, 0.7f}));
  object_add((Object *)floor_build(0));

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

  // waveform_t *sinewave = play_sinewave(0, 31100.0F, 10000);
  // waveform_t *bgm = play_mp3("rom:/theme_a_compressed.mp3", 0);

  rdpq_font_t *fnt1 = rdpq_font_load("rom:/Roboto-Bold.font64");
  debugf("fnt1 ptr: %p\n", fnt1);

  physics_init();

  uint32_t screen_w, screen_h;
  screen_w = display_get_width(); // cache these results. i don't think the
                                  // compiler will do that automatically? i
                                  // think a function call is too difficult to
                                  // statically analyse in most cases?
  screen_h = display_get_height();

  struct SplitScreenArea area_left = {0, 0, -1, screen_h},
                         area_right = {-1, 0, screen_w, screen_h};
  float area_left_right_divide_x = screen_w * 0.6f;

  while (true) {
    // get delta before anything else.
    uint32_t ticks_now = TICKS_READ();
    uint32_t ticks_elapsed = TICKS_DISTANCE(ticks_last, ticks_now);
    ticks_last = ticks_now;
    float seconds_elapsed = (float)ticks_elapsed / TICKS_PER_SECOND;

    // then, grab the input before we try to access old data.
    input_update();
    physics_update();
    object_update(); // update after the inputs have been processed.

    // draw after the objects have updated, don't draw old state.

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

      object_draw(); // when should this be called? how can we control the
                     // drawing context that the object is drawn in? this needs
                     // some more thought put into it. maybe the draw()
                     // lifecycle method takes in a DrawContext structure? how
                     // can we control which SCREEN it's drawn on, in
                     // particular? (for now, draw to the left.)

      // draw to the right area
      splitscreen_area_activate(&area_right);

      glClearColor(0, 0.2, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      // reuse the previous transform but with an additional rotation in between
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity(); // use a different one for the other screen.
      glprim_pyramid((vec3){0, 0, -20.0F});

      glprim_pyramid((vec3){-5.0F, 2.0F, -25.0F});

      glprim_pyramid((vec3){-1.0F, -5.0F, -30.0F});

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
      // move the divide between the left and right areas using C left/right
      if (input_state.left.pressed.C_left || input_state.left.pressed.C_right)
        area_left_right_divide_x +=
            (input_state.left.pressed.C_left ? -1 : 1) * 50 * seconds_elapsed;

      area_left_right_divide_x =
          clamp(area_left_right_divide_x, screen_w * SPLIT_MIN_PERCENT,
                screen_w * SPLIT_MAX_PERCENT);

      if (input_state.left.down.C_down)
        object_remove_by_ptr(player_obj);
    }

    m_audio_update();
  }

  // free(sinewave);
  // free(bgm);

  m_audio_clean();
  physics_clean();
  object_clean();
}
