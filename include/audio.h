#ifndef AUDIO_H
#define AUDIO_H

#include <libdragon.h>

enum SONG_TYPE { SONG_XM, SONG_YM };

typedef struct Song {
  bool is_active;

  uint32_t start_play_loop;
  int audiosz;
  const char *song_path;
} Song;

typedef struct AudioState {
  const int frequency;
  int numbuffers;
  int num_channels;
  Song curr_bgm;

} AudioState;

extern AudioState audio_state;

void m_audio_init();
void m_audio_update();
void m_audio_clean();
void m_audio_change_bgm(const char *song_path);

#endif // AUDIO_H
