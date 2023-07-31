#include "include/objects/floor.h"

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

Floor *floor_build(float y_pos) {
  Floor *f = (Floor *)malloc(sizeof(Floor));
  memcpy(f->o.position, (vec3){0, y_pos, 0},
         sizeof(float) * 3); // copy the literal from the stack.
  f->o.num_colliders = 1;
  f->o.colliders =
      (Collider *)malloc(sizeof(Collider) * 1); // space for one collider.
  f->o.colliders[0].type = CL_FLOOR;
  f->o.type = OBJ_FLOOR;
  return f;
}

// maybe don't have a separate destructor? is there a point to that?
void floor_clean(void *f) {
  Floor *floor = (Floor *)f;
  free(floor);
}
