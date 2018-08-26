#ifndef DE_MATHUTILS_H
#define DE_MATHUTILS_H

#include <cmath> /* ceil, max */
#include "BasicTypes.h"

namespace DE {

const f32 FLOAT_EPSILON = 0.00001f;

f32 rad(const f32 deg);
f32 deg(const f32 rad);

// f32 comparison

bool eqf(const f32 a, const f32 b, const f32 epsilon);
bool eqf(const f32 a, const f32 b); // default epsilon

f32 clamp(const f32 n, const f32 lower, const f32 upper);

}

#endif /* DE_MATHUTILS_H */
