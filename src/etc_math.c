#include "include/etc_math.h"

float clamp(float value, float min, float max) {
  return fminf(fmaxf(value, min), max);
}
