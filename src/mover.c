#include "include/mover.h"
#include "include/input.h"

#include <stdint.h>

#include "include/glprimitives.h"

#include <GL/gl.h>
#include <libdragon.h>

// use the dummy as the default entity, just for the initial setup of the object
// pipeline in the game.
Entity test_entity = {{0, -0.9F, 0}, 0.5F};

void entity_init() {}

void entity_update() {
  float x = input_state.left.pressed.x;
  float y = input_state.left.pressed.y;

  test_entity.position[0] += x * test_entity.speed;
  test_entity.position[2] -= y * test_entity.speed;
}

void entity_draw() { // always draw at the same place, modify position through
                     // matrices in the rendering pipeline.
  glprim_cube(test_entity.position);
}

void entity_clean() {}
