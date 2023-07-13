// SPDX-License-Identifier: CC0-1.0

#include <controller.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <libdragon.h>

#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <GL/glu.h>

#include "include/glprimitives.h"

void callback(float radius, int n_segments, int iy, int itheta) {
  glColor3f((float)iy / n_segments, (float)itheta / n_segments, 1.0f);
}

static bool strendswith(const char *str, const char *suffix) {
  char *p = strstr(str, suffix);
  return p && p[strlen(suffix)] == '\0';
}

void play_song(const char *song_path) { // setup audio
  enum SONG_TYPE { SONG_XM, SONG_YM };

  xm64player_t xm;
  ym64player_t ym;
  ym64player_songinfo_t yminfo;
  enum SONG_TYPE song_type;
  const char *song_name;
  int song_channels;
  int song_romsz = 0, song_ramsz = 0;

  if (strendswith(song_path, ".ym64") || strendswith(song_path, ".YM64"))
    song_type = SONG_YM;
  else
    song_type = SONG_XM;
  {
    int fh = dfs_open(song_path + 5);
    song_romsz = dfs_size(fh);
    dfs_close(fh);
  }

  debugf("Loading %s\n", song_path);
  if (song_type == SONG_XM) {
    xm64player_open(&xm, song_path);
    xm64player_play(&xm, 0);
  } else {
    ym64player_open(&ym, song_path, &yminfo);
    ym64player_play(&ym, 0);
  }
  //
  // // Unmute all channels
  // memset(mute, 0, sizeof(mute));
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

  rdpq_font_t *fnt1 = rdpq_font_load("rom:/Roboto-Medium.font64");
  debugf("fnt1 ptr: %p\n", fnt1);

  audio_init(44100, 4);
  mixer_init(32);

  // play song, setup loop vars
  enum SONG_TYPE { SONG_XM, SONG_YM };

  const char* song_path = "rom:/AQUA.xm64";

  xm64player_t xm;
  ym64player_t ym;
  ym64player_songinfo_t yminfo;
  enum SONG_TYPE song_type;
  const char *song_name;
  int song_channels;
  int song_romsz = 0, song_ramsz = 0;

  if (strendswith(song_path, ".ym64") || strendswith(song_path, ".YM64"))
    song_type = SONG_YM;
  else
    song_type = SONG_XM;
  {
    int fh = dfs_open(song_path + 5);
    song_romsz = dfs_size(fh);
    dfs_close(fh);
  }

  debugf("Loading %s\n", song_path);
  if (song_type == SONG_XM) {
    xm64player_open(&xm, song_path);
    xm64player_play(&xm, 0);
  } else {
    ym64player_open(&ym, song_path, &yminfo);
    ym64player_play(&ym, 0);
  }

  uint32_t start_play_loop = TICKS_READ();
  bool first_loop = true;
  int audiosz = audio_get_buffer_length();

  while (true) {
    // get delta
    uint32_t ticks_now = TICKS_READ();
    uint32_t ticks_elapsed = TICKS_DISTANCE(ticks_last, ticks_now);
    ticks_last = ticks_now;
    float seconds_elapsed = (float)ticks_elapsed / TICKS_PER_SECOND;

    // need to do this in the loop, or else we can't show twice on the same
    // display.
    surface_t *disp = display_get();

    rdpq_attach(disp, &zbuffer);
    { // gl loop
      gl_context_begin();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(80.0f, 4.0f / 3.0f, 0.1f, 10.0f);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
      glRotatef(rot_pitch, 1, 0, 0);
      glRotatef(rot_yaw, 0, 1, 0);

      glprim_sphere(radius, n_segments, callback);
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
    }

    { // audio processing loop, write the audio to the rsp.
      extern int64_t __mixer_profile_rsp, __wav64_profile_dma;
      __mixer_profile_rsp = __wav64_profile_dma = 0;

      uint32_t t0 = TICKS_READ();

      while (!audio_can_write()) {
      }

      uint32_t t1 = TICKS_READ();

      int16_t *out = audio_write_begin();
      mixer_poll(out, audiosz);
      audio_write_end();

      uint32_t t2 = TICKS_READ();

      first_loop = false;
    }
  }
}
