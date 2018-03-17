#include "MathUtils.h"
#include "BasicTypes.h"
#include <algorithm>

namespace DE {

f32 rad(const f32 deg){
  return deg * (f32)(M_PI/180.0f);
}

f32 deg(const f32 rad){
  return rad * (f32)(180.0f/M_PI);
}

bool eqf(const f32 a, const f32 b, const f32 epsilon){
  return fabsf(a-b) < epsilon;
}

bool eqf(const f32 a, const f32 b){
  return eqf(a, b, FLOAT_EPSILON);
}

f32 clamp(const f32 n, const f32 lower, const f32 upper) {
  return std::max(lower, std::min(n, upper));
}

}
