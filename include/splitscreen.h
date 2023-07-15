#ifndef SPLITSCREEN_H
#define SPLITSCREEN_H

struct SplitScreenArea {
  // bottom-left/top-right x/y
  short bl_x, bl_y, tr_x, tr_y;
};

void splitscreen_area_activate(struct SplitScreenArea *area);
void splitscreen_area_reset(void);

#endif
