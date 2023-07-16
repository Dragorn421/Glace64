// SPDX-License-Identifier: CC0-1.0

#include <GL/gl_enums.h>
#include <debug.h>

#include <math.h>
#include <string.h>

#include <GL/gl.h>

#include "include/render_defines.h"

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

void glprim_sphere(float radius, int n_segments,
                   void (*callback)(float, int, int, int)) {
  float buffer_a[n_segments][3];
  float buffer_b[n_segments][3];

  float(*pos_prev)[3] = buffer_a;
  float(*pos_cur)[3] = buffer_b;

  for (int iy = 0; iy <= n_segments; iy++) {
    float y = ((float)iy / n_segments - 0.5f) * 2.0f * radius;
    float xz = sqrtf(radius * radius - y * y);

    for (int itheta = 0; itheta < n_segments; itheta++) {
      float theta = (float)itheta / n_segments * 2.0f * M_PI;
      // Rotate around +y
      pos_cur[itheta][0] = xz * cosf(theta);
      pos_cur[itheta][1] = y;
      pos_cur[itheta][2] = xz * -sinf(theta);
    }

    if (iy != 0) {
      glBegin(GL_QUAD_STRIP);
      // pos_prev is the circle of vertices at the previous y increment (below)
      // pos_cur is above
      // Knowing the specification for GL_QUAD_STRIP, to put the faces oriented
      // outwards, this means we send the pos_prev vertex first for each segment
      // of the strip
      for (int itheta = 0; itheta < n_segments; itheta++) {
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

// data for a basic cube model.
static const Vertex cube_vertices[] = {
    // top
    {.position = {0.5, 0.5, 2},
     .texcoord = {1.024f, 0.992f},
     .normal = {0.f, 0.f, 1.f},
     .color = 0xFF0000FF}, /* 0 */
    {.position = {-0.5, 0.5, 2},
     .texcoord = {0.f, 0.992f},
     .normal = {0.f, 0.f, 1.f},
     .color = 0xFF0000FF}, /* 1 */
    {.position = {-0.5, -0.5, 2},
     .texcoord = {0.f, 0.512f},
     .normal = {0.f, 0.f, 1.f},
     .color = 0xFF0000FF}, /* 2 */
    {.position = {0.5, -0.5, 2},
     .texcoord = {1.024f, 0.512f},
     .normal = {0.f, 0.f, 1.f},
     .color = 0xFF0000FF}, /* 3 */

    {.position = {0.5, -0.5, 0},
     .texcoord = {1.024f, 1.112f},
     .normal = {0.f, -1.f, 0.f},
     .color = 0x00FFFFFF}, /* 4 */
    {.position = {0.5, -0.5, 2},
     .texcoord = {1.024f, 0.064f},
     .normal = {0.f, -1.f, 0.f},
     .color = 0x00FFFFFF}, /* 5 */
    {.position = {-0.5, -0.5, 2},
     .texcoord = {0.f, 0.064f},
     .normal = {0.f, -1.f, 0.f},
     .color = 0x00FFFFFF}, /* 6 */
    {.position = {-0.5, -0.5, 0},
     .texcoord = {0.f, 1.112f},
     .normal = {0.f, -1.f, 0.f},
     .color = 0x00FFFFFF}, /* 7 */

    {.position = {-0.5, -0.5, 0},
     .texcoord = {1.024f, 0.676f},
     .normal = {-1.f, 0.f, 0.f},
     .color = 0x00FF00FF}, /* 8 */
    {.position = {-0.5, -0.5, 2},
     .texcoord = {1.024f, 1.056f},
     .normal = {-1.f, 0.f, 0.f},
     .color = 0x00FF00FF}, /* 9 */
    {.position = {-0.5, 0.5, 2},
     .texcoord = {0.f, 1.056f},
     .normal = {-1.f, 0.f, 0.f},
     .color = 0x00FF00FF}, /* 10 */
    {.position = {-0.5, 0.5, 0},
     .texcoord = {0.f, 0.676f},
     .normal = {-1.f, 0.f, 0.f},
     .color = 0x00FF00FF}, /* 11 */

    // down
    {.position = {-0.5, 0.5, 0},
     .texcoord = {0.960f, 0.576f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FFFF}, /* 12 */
    {.position = {0.5, 0.5, 0},
     .texcoord = {0.960f, 0.896f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FFFF}, /* 13 */
    {.position = {0.5, -0.5, 0},
     .texcoord = {0.064f, 0.896f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FFFF}, /* 14 */
    {.position = {-0.5, -0.5, 0},
     .texcoord = {0.064f, 0.576f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FFFF}, /* 15 */

    {.position = {0.5, 0.5, 0},
     .texcoord = {1.024f, 0.992f},
     .normal = {1.f, 0.f, 0.f},
     .color = 0x0000FFFF}, /* 16 */
    {.position = {0.5, 0.5, 2},
     .texcoord = {1.024f, 0.928f},
     .normal = {1.f, 0.f, 0.f},
     .color = 0x0000FFFF}, /* 17 */
    {.position = {0.5, -0.5, 2},
     .texcoord = {0.f, 0.928f},
     .normal = {1.f, 0.f, 0.f},
     .color = 0x0000FFFF}, /* 18 */
    {.position = {0.5, -0.5, 0},
     .texcoord = {0.f, 0.992f},
     .normal = {1.f, 0.f, 0.f},
     .color = 0x0000FFFF}, /* 19 */

    {.position = {-0.5, 0.5, 0},
     .texcoord = {1.024f, 0.648f},
     .normal = {0.f, 1.f, 0.f},
     .color = 0xFFFF00FF}, /* 20 */
    {.position = {-0.5, 0.5, 2},
     .texcoord = {1.024f, 0.928f},
     .normal = {0.f, 1.f, 0.f},
     .color = 0xFFFF00FF}, /* 21 */
    {.position = {0.5, 0.5, 2},
     .texcoord = {0.f, 0.928f},
     .normal = {0.f, 1.f, 0.f},
     .color = 0xFFFF00FF}, /* 22 */
    {.position = {0.5, 0.5, 0},
     .texcoord = {0.f, 0.648f},
     .normal = {0.f, 1.f, 0.f},
     .color = 0xFFFF00FF}, /* 23 */
};

static const uint16_t cube_indices[] = {
    0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
    12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
};

// uses a vao and ibo, unlike the sphere.
void glprim_cube(float position[3]) {
  glPushMatrix();
  // don't need to do any linear algebra here, since we're using a fixed camera
  // position for the left screen? i think?
  glTranslatef(position[0], position[1],
               position[2]); // translate the modelview by the right
                             // test_entity position, locally.
  glScalef(1.f, 1.f, 1.f);

  glEnableClientState(GL_VERTEX_ARRAY); // using vaos and ibos!!
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  // the vertex structure just happens to match the vertex array layout that
  // we're using here.
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex),
                  (void *)(0 * sizeof(float) + (void *)cube_vertices));
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex),
                    (void *)(3 * sizeof(float) + (void *)cube_vertices));
  glNormalPointer(GL_FLOAT, sizeof(Vertex),
                  (void *)(5 * sizeof(float) + (void *)cube_vertices));
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex),
                 (void *)(8 * sizeof(float) + (void *)cube_vertices));

  glDrawElements(GL_TRIANGLES, sizeof(cube_indices) / sizeof(uint16_t),
                 GL_UNSIGNED_SHORT, cube_indices);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glPopMatrix();
}
