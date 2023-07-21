#ifndef GLPRIMITIVES_H
#define GLPRIMITIVES_H

#include "include/render_defines.h"

// shape renderers
void glprim_sphere(float radius, int n_segments,
                   void (*callback)(float, int, int, int));
void glprim_cube(float position[3]);
void glprim_pyramid(float position[3]);

// then, the generic rendering methods over vertex pointers.
void glprim_fullvtx(FullRender *v, float position[3]);

#endif
