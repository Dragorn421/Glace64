#ifndef GLPRIMITIVES_H
#define GLPRIMITIVES_H

#include "include/cglm/types.h"
#include "include/render_defines.h"

// shape renderers
void glprim_sphere(float radius, int n_segments,
                   void (*callback)(float, int, int, int));
void glprim_cube(vec3 position);
void glprim_pyramid(vec3 position);

// then, the generic rendering methods over vertex pointers.
void glprim_fullvtx(FullRender *v, vec3 position);

#endif
