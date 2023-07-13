#include "include/audio.h"

#include <stdlib.h>

// avoid malloc, just write the state into the .data of the ofile?
AudioState as = {
    44100,
    4,
    32,
    {
      0,
      0,
      0,
      "",
    }
};

static bool strendswith(const char *str, const char *suffix) {
  char *p = strstr(str, suffix);
  return p && p[strlen(suffix)] == '\0';
}

// lifetime module functions
void m_audio_init() {
  audio_init(44100, 4);
  mixer_init(32);
}

static void update_single_track(Song s) {
  if (TICKS_DISTANCE(s.start_play_loop, TICKS_READ()) < TICKS_PER_SECOND) {
    s.is_active = false;
    return;
  }

  extern int64_t __mixer_profile_rsp, __wav64_profile_dma;
  __mixer_profile_rsp = __wav64_profile_dma = 0;

  uint32_t t0 = TICKS_READ();

  while (!audio_can_write()) {
  }

  uint32_t t1 = TICKS_READ();

  int16_t *out = audio_write_begin();
  mixer_poll(out, s.audiosz);
  audio_write_end();

  uint32_t t2 = TICKS_READ();
}

void m_audio_update() { // audio processing loop, write the audio to the rsp.
  if (as.curr_bgm.is_active) { // i'm not sure how many tracks we need
                             // concurrently, i'm just using the one bgm track
                             // now but more could be added to the structure.
    debugf("song updating: addr %p, at path %s, with audiosz %d.\n", &as.curr_bgm, as.curr_bgm.song_path, as.curr_bgm.audiosz);
    update_single_track(as.curr_bgm);
  }
}

void m_audio_clean() {}

// then exposed helper functions for the audio subsystem.
void m_audio_change_bgm(const char *song_path) {
  // since the pointer is still malloced and not NULL, we can't just compare the
  // curr_bgm to NULL. use an is_active field? is there a cleaner way of doing
  // this?
  if (as.curr_bgm.is_active) { // don't restart the song if it's played again
                                // while it's already active.
    if (strcmp(as.curr_bgm.song_path, song_path)) {
      debugf("[Audio] Tried to play the same song twice, ignoring command... "
             "(%s)",
             song_path);
    }
  }

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

  debugf("[Audio] Loading %s...\n", song_path);
  if (song_type == SONG_XM) {
    xm64player_open(&xm, song_path);
    xm64player_play(&xm, 0);
  } else {
    ym64player_open(&ym, song_path, &yminfo);
    ym64player_play(&ym, 0);
  }

  uint32_t start_play_loop = TICKS_READ();
  int audiosz = audio_get_buffer_length();
  // memcpy from the struct literal stack instance to the malloced heap Song*,
  // directly into the AudioState* to avoid re-mallocing unnecessarily.

  as.curr_bgm = (Song){
     true, //initially active.
     start_play_loop,
     audiosz,
     song_path,
  };
}
