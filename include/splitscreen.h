#ifndef SPLITSCREEN_H
#define SPLITSCREEN_H

// the max and min of the screen split distance.
#define SPLIT_MIN_PERCENT 0.3F
#define SPLIT_MAX_PERCENT 0.7F

struct SplitScreenArea {
  // bottom-left/top-right x/y
  short bl_x, bl_y, tr_x, tr_y;
};

void splitscreen_area_activate(struct SplitScreenArea *area);
void splitscreen_area_reset(void);

#endif
