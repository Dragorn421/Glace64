#include <limits.h>
#include <stdint.h>

#include <libdragon.h>

#include "include/ourmath.h"
#include "include/physics.h"

float tick_seconds;
int tick_microseconds;

static uint32_t next_tick_start_cputicks;

static void physics_set_tick_duration(int tick_duration_us) {
  tick_duration_us =
      CLAMP(tick_duration_us, MIN_TICK_DURATION_US, MAX_TICK_DURATION_US);
  tick_seconds = (float)tick_duration_us / MICROSECONDS_PER_SECOND;
  tick_microseconds = tick_duration_us;
}

void physics_reset_tick(void) {
  next_tick_start_cputicks = TICKS_READ();
  // two justifications for using the minimum as the initial value:
  // - it means initial minimal change from the "previous" (non-existant) tick
  // - it's the same as calling physics_next_tick() immediately after
  //    physics_reset_tick() anyway
  physics_set_tick_duration(MIN_TICK_DURATION_US);
}

void physics_next_tick(void) {
  uint32_t start_cputicks = next_tick_start_cputicks;
  uint32_t end_cputicks = TICKS_READ();

  int32_t elapsed_cputicks = TICKS_DISTANCE(start_cputicks, end_cputicks);
  float seconds_elapsed = (float)elapsed_cputicks / TICKS_PER_SECOND;
  int us_elapsed =
      FLOAT_TO_INT_CHECK(seconds_elapsed * MICROSECONDS_PER_SECOND);

  // Also handles us_elapsed being negative if next_tick_start_cputicks was set
  // in the future
  if (us_elapsed < MIN_TICK_DURATION_US) {
    physics_set_tick_duration(MIN_TICK_DURATION_US);
    // Ensure the real time tracking of ticks corresponds to the physics tick
    // duration (like in the general MIN_TICK_DURATION_US <= us_elapsed <=
    // MAX_TICK_DURATION_US case)
    // Note: this sets next_tick_start_cputicks in the future
    next_tick_start_cputicks = start_cputicks + (uint64_t)MIN_TICK_DURATION_US *
                                                    TICKS_PER_SECOND /
                                                    MICROSECONDS_PER_SECOND;
  } else if (us_elapsed > MAX_TICK_DURATION_US) {
    // Have the physics tick duration be the capped time, but don't use the
    // capped time for tracking real time. Otherwise the game would speed up
    // later to catch up.
    physics_set_tick_duration(MAX_TICK_DURATION_US);
    next_tick_start_cputicks = end_cputicks;
  } else {
    physics_set_tick_duration(us_elapsed);
    // Without the uint64_t cast, the multiplication overflows int=int32_t
    // Note there is some precision loss here on the division so we
    // (lose/gain ?) some time, but it should not be noticeable at all
    next_tick_start_cputicks = start_cputicks + (uint64_t)us_elapsed *
                                                    TICKS_PER_SECOND /
                                                    MICROSECONDS_PER_SECOND;
  }
}
