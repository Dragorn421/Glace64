#ifndef OURMATH_H
#define OURMATH_H

#include <debug.h>
#include <limits.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

// Clamp v to the range [a;b]
#define CLAMP(v, a, b)                                                                  \
  ({                                                                                    \
    /* (typeof is a GNU extension, see the cpp manual:                                  \
     *  https://gcc.gnu.org/onlinedocs/gcc-13.2.0/cpp/Duplication-of-Side-Effects.html) \
     */                                                                                 \
    typeof(v) v_ = (v);                                                                 \
    typeof(a) a_ = (a);                                                                 \
    typeof(b) b_ = (b);                                                                 \
    v_<a_ ? a_ : v_> b_ ? b_ : v_;                                                      \
  })

#define MIN(a, b)                                                              \
  ({                                                                           \
    typeof(a) a_ = (a);                                                        \
    typeof(b) b_ = (b);                                                        \
    a_ < b_ ? a_ : b_;                                                         \
  })
#define MAX(a, b)                                                              \
  ({                                                                           \
    typeof(a) a_ = (a);                                                        \
    typeof(b) b_ = (b);                                                        \
    a_ > b_ ? a_ : b_;                                                         \
  })

#define FLOAT_TO_INT_CHECK(f)                                                  \
  ({                                                                           \
    float f_ = (f);                                                            \
    assertf(f_ >= INT_MIN && f_ <= INT_MAX, "float does not fit into int: %f", \
            f_);                                                               \
    (int)f_;                                                                   \
  })

#endif
