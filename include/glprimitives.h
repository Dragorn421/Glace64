#ifndef GLPRIMITIVES_H
#define GLPRIMITIVES_H

void glprim_sphere(float radius, int n_segments,
                   void (*callback)(float, int, int, int));
void glprim_cube(float position[3]);

#endif
