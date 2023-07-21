#ifndef RENDER_DEFINES_H
#define RENDER_DEFINES_H

#include "include/cglm/types.h"
#include <stdint.h>

typedef struct {
  vec3 position;
  uint32_t color;
} BasicVertex;

typedef struct BasicRender {
  BasicVertex *v;
  uint16_t *indices;
} BasicRender;

typedef struct { // use a gl-vertex layout oriented structure, rather than the
                 // normal ultra64.h Vtx union structure. this is actually the
                 // same layout as Vtx_n? probably not a coincidence?
  vec3 position;
  uint32_t color;
  float texcoord[2];
  float normal[3];
} FullVertex;

// really, we should always be using indices.
// glprim specifies basic rendering routines for all of these scenarios.
typedef struct FullRender {
  FullVertex *vertices; // list of vertices
  uint16_t *indices;    // list of indices
  uint32_t num_indices; // number of indices in the list
} FullRender;

#endif // DEBUG
