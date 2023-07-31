#include "include/object_lut.h"

#include "include/object.h"
#include "include/objects/cube.h"
#include "include/objects/floor.h"
#include "include/objects/player.h"
#include "include/objects/sphere.h"

// do-nothing table functions. used as placeholders.
void noop() {}

#define TODO                                                                   \
  { noop, noop, noop, noop, noop }

// define the actual LUT object.
ObjectFnPointers fn_lut[] = {
    // for example, we'll do fn_lut[o->type].update(o); to call the update
    // function. minimize icache misses by deferring behavior to different
    // functions, rather than inlining in a huge switch.
    [OBJ_PLAYER] =
        {player_init, player_update, player_draw, player_clean,
         player_handle_collision}, // register event handler functions here too.
    [OBJ_CUBE] =
        {cube_init, cube_update, cube_draw, cube_clean,
         cube_handle_collision}, // register event handler functions here too.
    [OBJ_FLOOR] = {noop, noop, noop, floor_clean,
                   noop}, // register event handler functions here too.
    [OBJ_CRAB] = TODO,
    [OBJ_SPHERE] = {objectsphere_init, objectsphere_update, objectsphere_draw,
                    objectsphere_clean, noop},
};
