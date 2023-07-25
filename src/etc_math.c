#include "include/etc_math.h"

int min(int a, int b) { return (a < b) ? a : b; }

float clamp(float value, float min, float max) {
  return fminf(fmaxf(value, min), max);
}
