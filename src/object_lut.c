#include "include/object_lut.h"

#include "include/objects/player.h"

// do-nothing table functions. used as placeholders.
void none(void *v) {}
void none_col(void *v, CollisionEvent *e) {}

#define TODO                                                                   \
  { none, none, none, none, none_col }

// define the actual LUT object.
ObjectFnPointers fn_lut[] = {
    // for example, we'll do fn_lut[o->type].update(o); to call the update
    // function. minimize icache misses by deferring behavior to different
    // functions, rather than inlining in a huge switch.
    [OBJ_PLAYER] =
        {player_init, player_update, player_draw, player_clean,
         player_handle_collision}, // register event handler functions here too.
    [OBJ_CRAB] = TODO,
};
