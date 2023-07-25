#ifndef MP3_H
#define MP3_H

#include <libdragon.h>

waveform_t *play_mp3(const char *song_path, int channel);
void mp3_update(); // call this in a loop to fill the pcm channel. or else the
                   // callback will have to do it itself and it'll sound bad,
                   // since it needs to wait for the decoding to fill the buffer
                   // before it can write the bytes.

#endif // !MP3_H
