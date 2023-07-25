// example of a custom libdragon audio subsystem, which plays a mono 16-bit
// sinewave.
#include "include/audio/simple_sinewave.h"
#include "mixer.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846
#define FREQUENCY 440.0   // Frequency of the sine wave (in Hz)
#define SAMPLE_RATE 44100 // Sample rate of the audio (in Hz)
#define MAX_AMP 0x7FFF    // Maximum amplitude for a 16-bit PCM signal

static void generate_sine_wave(int16_t *buffer, int len) {
  for (int i = 0; i < len; i++) {
    // Generate the sine value (ranging from -1.0 to 1.0)
    double sine_val = sin(2.0 * PI * FREQUENCY * ((double)i / SAMPLE_RATE));

    // Convert the sine value to a 16-bit PCM sample
    buffer[i] = (int16_t)(sine_val * MAX_AMP);
  }
}

static void waveform_read(void *ctx, samplebuffer_t *sbuf, int wpos,
                          int wlen, // it'll tell us how many samples to write.
                          bool seeking) {
  // Get a pointer to the area in the buffer where the new samples can be
  // written. basically using the mixer api to malloc space in the audio buffer.
  int16_t *buffer_area = (int16_t *)samplebuffer_append(sbuf, wlen);

  // 16-bit precision pcm wave samples.

  // this works! just write to the raw pointer, then load in the audio.
  generate_sine_wave(buffer_area, wlen);
}

// request that the AI play a sinewave.
waveform_t *play_sinewave(int channel, float frequency, int len) {
  waveform_t *my_wave = (waveform_t *)malloc(sizeof(waveform_t));
  memcpy(my_wave,
         &(waveform_t){
             .name = "My Waveform",
             .bits = 16,             // 16 bits per sample
             .channels = 1,          // mono audio
             .frequency = frequency, // 44100 Hz (CD quality audio)
             .len = len,             // length of your audio data
             .loop_len = 0,          // don't loop
             .read = waveform_read,  // your sample reading function
             .ctx = 0 // the context for the read function, doesn't need it in
                      // this case.
         },
         sizeof(waveform_t));

  mixer_ch_play(channel, my_wave);
  return my_wave;
}
