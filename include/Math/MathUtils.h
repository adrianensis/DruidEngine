#ifndef DE_MATHUTILS_H
#define DE_MATHUTILS_H

#include <cmath> /* ceil, max */
#include "BasicTypes.h"

namespace DE {

const f32 FLOAT_EPSILON = 0.00001f;

const f32 DE_PI        = acosf(-1.0f); // pi
const f32 DE_PI_2      = DE_PI/2.0f; // pi/2
const f32 DE_PI_4      = DE_PI/4.0f; // pi/4
const f32 DE_1_PI      = 1.0f/DE_PI; // 1/pi
const f32 DE_2_PI      = 2.0f/DE_PI; // 2/pi
const f32 DE_2_SQRTPI  = 2/sqrtf(DE_PI); // 2/sqrt(pi)
const f32 DE_SQRT2     = sqrtf(2.0f); // sqrt(2)
const f32 DE_SQRT1_2   = 1.0f/sqrtf(2.0f); // 1/sqrt(2)
const f32 DE_E         = 2.71828182845904523536; // e
const f32 DE_LOG2E     = log2(DE_E); // log_2(e)
const f32 DE_LOG10E    = log10(DE_E); // log_10(e)
const f32 DE_LN2       = log1p(2.0f); // log_e(2)
const f32 DE_LN10      = log1p(10.0f); // log_e(10)

const f32 TO_RAD    = DE_PI/180.0f;
const f32 TO_DEG    = 180.0f/DE_PI;

f32 rad(const f32 deg);
f32 deg(const f32 rad);

// f32 comparison

bool eqf(const f32 a, const f32 b, const f32 epsilon);
bool eqf(const f32 a, const f32 b); // default epsilon

f32 clamp(const f32 n, const f32 lower, const f32 upper);

}

#endif /* DE_MATHUTILS_H */
