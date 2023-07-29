#include "include/audio/mp3.h"
#include "include/ourmath.h"
#include "include/util.h"
#include "mixer.h"
#include <stdint.h>
#include <stdlib.h>

#define SAMPLE_RATE 16000 // Common sample rate for audio CDs

#define MINIMP3_IMPLEMENTATION
#include "include/deps/minimp3_ex.h"

// how much we'll store in the pcm buffer to prevent audio buffering as it's
// loading in real time.
// * 2: 2 bytes per sample, i16
#define PCM_BUF_LEN (4000 * 2)
// how much we'll fill the pcm buffer each frame.
#define PCM_FILL_RATE (MINIMP3_MAX_SAMPLES_PER_FRAME)

typedef struct Decoder {
  mp3dec_ex_t mp3d;
  float mp3_duration;

  unsigned int pcm_pos;

  uint8_t pcm_buffer[PCM_BUF_LEN]; // 16-bit audio.
} Decoder;

void decode_samples(Decoder *dec, uint8_t *buf, int bytes) {
  memset(buf, 0, bytes);
  mp3dec_ex_read(&dec->mp3d, (mp3d_sample_t *)buf,
                 bytes /
                     sizeof(mp3d_sample_t)); // casting buf directly back to a
                                             // i16* sample, get bytes samples
                                             // from the decoder object.
}

int open_dec(Decoder *dec, const char *file_name) {
  if (!dec || !file_name || !*file_name)
    return 0;

  memset(dec, 0, sizeof(*dec)); // calloc decoder object

  mp3dec_ex_open(&dec->mp3d, file_name, MP3D_SEEK_TO_SAMPLE);
  if (!dec->mp3d.samples)
    return 0;

  return 1;
}

int close_dec(Decoder *dec) {
  mp3dec_ex_close(&dec->mp3d);
  memset(dec, 0, sizeof(*dec));
  return 0;
}

// waveform_read now uses the decoder struct and decode_samples function
static void waveform_read(void *ctx, samplebuffer_t *sbuf, int wpos, int wlen,
                          bool seeking) {
  debugf("AI requests %d samples from the mp3 decoder.\n", wlen);

  Decoder *d = (Decoder *)ctx;

  if (d->pcm_pos + wlen * 2 > PCM_BUF_LEN) {
    debugf("Decoding more of the mp3, running low on buffer...\n");
    int remaining = PCM_BUF_LEN - d->pcm_pos;
    memmove(d->pcm_buffer, d->pcm_buffer + d->pcm_pos, remaining);
    decode_samples(d, d->pcm_buffer + remaining, d->pcm_pos);
    d->pcm_pos = 0;
  }

  int16_t *buffer_area = (int16_t *)samplebuffer_append(sbuf, wlen);
  memcpy(buffer_area, d->pcm_buffer + d->pcm_pos,
         wlen * 2); // dump wlen * 2 bytes of sample data from the buffer.
  d->pcm_pos += wlen * 2;
}

waveform_t *play_mp3(const char *song_path, int channel) {
  waveform_t *my_wave = (waveform_t *)malloc(sizeof(waveform_t));

  // pass the mp3 decoder itself as context to the write callback.
  Decoder *dec = (Decoder *)malloc(sizeof(Decoder));
  if (!open_dec(
          dec,
          (const char *)song_path)) { // use open_dec to initialize the decoder

    debugf("Failed to initialize decoder for file [%s].\n",
           (const char *)song_path);
    return my_wave;
  } else {
    debugf("Opened mp3 decoder for file [%s].\n", (const char *)song_path);
  }

  memcpy(my_wave,
         &(waveform_t){.name = "My Waveform",
                       .bits = 16,
                       .channels = 1,
                       .frequency = SAMPLE_RATE,

                       .len = WAVEFORM_UNKNOWN_LEN,

                       .loop_len = 0, // yes, loop it by default.

                       .read = waveform_read,

                       .ctx = (void *)dec},
         sizeof(waveform_t));
  mixer_ch_play(
      channel,
      my_wave); // point the AI at this waveform. when we poll the mixer, it'll
                // query the callback once it's ready in m_audio_update()

  decode_samples(dec, dec->pcm_buffer, PCM_BUF_LEN);

  return my_wave;
}

void close_mp3() {}

// keep the mp3 buffer full.
void mp3_update() {}
