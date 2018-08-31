#include "MathUtils.h"
#include <algorithm>

namespace DE {

const f32 MathUtils::FLOAT_EPSILON = 0.00001f;

const f32 MathUtils::PI        = acosf(-1.0f); // pi
const f32 MathUtils::PI_2      = PI/2.0f; // pi/2
const f32 MathUtils::PI_4      = PI/4.0f; // pi/4
const f32 MathUtils::INV_PI      = 1.0f/PI; // 1/pi
const f32 MathUtils::INV_2_PI      = 2.0f/PI; // 2/pi
const f32 MathUtils::INV_2_SQRTPI  = 2/sqrtf(PI); // 2/sqrt(pi)
const f32 MathUtils::SQRT2         = sqrtf(2.0f); // sqrt(2)
const f32 MathUtils::INV_SQRT2   = 1.0f/sqrtf(2.0f); // 1/sqrt(2)
const f32 MathUtils::E         = 2.71828182845904523536; // e
const f32 MathUtils::LOG2E     = log2(E); // log_2(e)
const f32 MathUtils::LOG10E    = log10(E); // log_10(e)
const f32 MathUtils::LN2       = log1p(2.0f); // log_e(2)
const f32 MathUtils::LN10      = log1p(10.0f); // log_e(10)

const f32 MathUtils::PI_180    = PI/180.0f;
const f32 MathUtils::INV_PI_180    = 180.0f/PI;

f32 MathUtils::rad(const f32 deg){
  return deg * PI_180;
}

f32 MathUtils::deg(const f32 rad){
  return rad * INV_PI_180;
}

bool MathUtils::eqf(const f32 a, const f32 b, const f32 epsilon){
  return fabsf(a-b) < epsilon;
}

bool MathUtils::eqf(const f32 a, const f32 b){
  return eqf(a, b, FLOAT_EPSILON);
}

f32 MathUtils::clamp(const f32 n, const f32 lower, const f32 upper) {
  return std::max(lower, std::min(n, upper));
}

}
