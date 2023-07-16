#ifndef RENDER_DEFINES_H
#define RENDER_DEFINES_H

#include <stdint.h>

typedef struct { // use a gl-vertex layout oriented structure, rather than the
                 // normal ultra64.h Vtx union structure. this is actually the
                 // same layout as Vtx_n? probably not a coincidence?
  float position[3];
  float texcoord[2];
  float normal[3];
  uint32_t color;
} Vertex;

#endif // DEBUG
