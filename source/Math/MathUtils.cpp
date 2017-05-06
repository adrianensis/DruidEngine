#include "MathUtils.h"
#include <algorithm>

namespace DE {

float rad(const float& deg){
  return deg * (float)(M_PI/180.0f);
}

float deg(const float& rad){
  return rad * (float)(180.0f/M_PI);
}

bool eqf(const float& a, const float& b, const float& epsilon){
  return fabsf(a-b) < epsilon;
}

bool eqf(const float& a, const float& b){
  return eqf(a, b, FLOAT_EPSILON);
}

float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

}
