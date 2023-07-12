// SPDX-License-Identifier: CC0-1.0

#include <debug.h>

#include <math.h>
#include <string.h>

#include <GL/gl.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

void glprim_sphere(float radius, int n_segments, void (*callback)(float, int, int, int))
{
    float buffer_a[n_segments][3];
    float buffer_b[n_segments][3];

    float(*pos_prev)[3] = buffer_a;
    float(*pos_cur)[3] = buffer_b;

    for (int iy = 0; iy <= n_segments; iy++)
    {
        float y = ((float)iy / n_segments - 0.5f) * 2.0f * radius;
        float xz = sqrtf(radius * radius - y * y);

        for (int itheta = 0; itheta < n_segments; itheta++)
        {
            float theta = (float)itheta / n_segments * 2.0f * M_PI;
            // Rotate around +y
            pos_cur[itheta][0] = xz * cosf(theta);
            pos_cur[itheta][1] = y;
            pos_cur[itheta][2] = xz * -sinf(theta);
        }

        if (iy != 0)
        {
            glBegin(GL_QUAD_STRIP);
            // pos_prev is the circle of vertices at the previous y increment (below)
            // pos_cur is above
            // Knowing the specification for GL_QUAD_STRIP, to put the faces oriented outwards,
            // this means we send the pos_prev vertex first for each segment of the strip
            for (int itheta = 0; itheta < n_segments; itheta++)
            {
                if (callback != NULL)
                    callback(radius, n_segments, iy, itheta);
                glVertex3fv(pos_prev[itheta]);
                glVertex3fv(pos_cur[itheta]);
            }
            // close the loop
            if (callback != NULL)
                callback(radius, n_segments, iy, n_segments);
            glVertex3fv(pos_prev[0]);
            glVertex3fv(pos_cur[0]);
            glEnd();
        }

        pos_prev = pos_cur;
        pos_cur = pos_prev == buffer_a ? buffer_b : buffer_a;
    }
}
