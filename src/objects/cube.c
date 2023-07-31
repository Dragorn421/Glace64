#include "include/objects/cube.h"

#include "include/cglm/types.h"
#include "include/cglm/vec3.h"
#include "include/glprimitives.h"
#include "include/input.h"
#include "include/object.h"
#include "include/physics.h"

#include <GL/gl.h>
#include <libdragon.h>
#include <stdint.h>
#include <stdlib.h>

Cube *cube_build(vec3 position) {
  Cube *c = (Cube *)malloc(sizeof(Cube));
  memcpy(c->o.position, position,
         sizeof(float) * 3); // copy the literal from the stack.
  c->o.num_colliders = 1;
  c->o.colliders =
      (Collider *)malloc(sizeof(Collider) * 1); // space for one collider.
  c->o.colliders[0].type = CL_PILLAR;
  c->o.type = OBJ_CUBE;
  c->speed = 0.3F;
  return c;
}

void cube_init(void *c) {}

void cube_update(void *c) {}

void cube_draw(void *c) { // always draw at the same place, modify position
                          // through matrices in the rendering pipeline.
  Cube *cube = (Cube *)c;
  glprim_cube(cube->o.position);
}

void cube_handle_collision(void *c, CollisionEvent *e) {
  Cube *cube = (Cube *)c;
  glm_vec3_scale(e->normalized_force, e->magnitude, e->normalized_force);
  glm_vec3_add(cube->o.position, e->normalized_force, cube->o.position);
}

// maybe don't have a separate destructor? is there a point to that?
void cube_clean(void *c) {
  Cube *cube = (Cube *)c;
  free(cube);
}
