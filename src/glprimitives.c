// SPDX-License-Identifier: CC0-1.0

#include <GL/gl_enums.h>
#include <debug.h>

#include <math.h>
#include <string.h>

#include <GL/gl.h>

#include "include/glprimitives.h"

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

// all vertices specify a position, and are offset by that position in the
// modelview. use a push-pop design for the function.
void glprim_fullvtx(FullRender *v, vec3 position) {
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
  glVertexPointer(3, GL_FLOAT, sizeof(FullVertex),
                  (void *)(0 * sizeof(float) + (void *)v->vertices));
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(FullVertex),
                 (void *)(3 * sizeof(float) + (void *)v->vertices));
  glNormalPointer(GL_FLOAT, sizeof(FullVertex),
                  (void *)(4 * sizeof(float) + (void *)v->vertices));
  glTexCoordPointer(2, GL_FLOAT, sizeof(FullVertex),
                    (void *)(7 * sizeof(float) + (void *)v->vertices));

  // is there a better way to do this than specifying the count directly?
  glDrawElements(GL_TRIANGLES, v->num_indices, GL_UNSIGNED_SHORT, v->indices);

  glDisableClientState(
      GL_VERTEX_ARRAY); // do we need to disable these? we're just going to use
                        // the same layout for most of the polys, right?
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glPopMatrix();
}

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

static const FullVertex cube_vertices[] = {
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

FullRender cube_render = {
    (FullVertex *)cube_vertices,
    (uint16_t *)cube_indices,
    sizeof(cube_indices) / sizeof(uint16_t),
};

static const FullVertex pyramid_vertices[] = {
    // base
    {.position = {-1.f, -1.f, 0.f},
     .texcoord = {0.f, 0.f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FF00}, /* 0 */
    {.position = {1.f, -1.f, 0.f},
     .texcoord = {1.f, 0.f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FF00}, /* 1 */
    {.position = {1.f, 1.f, 0.f},
     .texcoord = {1.f, 1.f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FF00}, /* 2 */
    {.position = {-1.f, 1.f, 0.f},
     .texcoord = {0.f, 1.f},
     .normal = {0.f, 0.f, -1.f},
     .color = 0xFF00FF00}, /* 3 */

    // top
    {.position = {0.f, 0.f, 2.f},
     .texcoord = {0.5f, 0.5f},
     .normal = {0.f, 0.f, 1.f},
     .color = 0xFFFF00FF}, /* 4 */
};

static const uint16_t pyramid_indices[] = {
    0, 1, 4,          // front face
    1, 2, 4,          // right face
    2, 3, 4,          // back face
    3, 0, 4,          // left face
    0, 1, 2, 0, 2, 3, // bottom face
};

FullRender pyramid_render = {
    (FullVertex *)pyramid_vertices,
    (uint16_t *)pyramid_indices,
    sizeof(pyramid_indices) / sizeof(uint16_t),
};

// uses a vao and ibo, unlike the sphere.
void glprim_cube(vec3 position) { glprim_fullvtx(&cube_render, position); }

void glprim_pyramid(vec3 position) {
  glprim_fullvtx(&pyramid_render, position);
}
