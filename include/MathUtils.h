#ifndef INCLUDE_MATHUTILS_H_
#define INCLUDE_MATHUTILS_H_

#include <cmath>

namespace DE {

const float FLOAT_EPSILON = 0.00001f;

float rad(const float& deg);
float deg(const float& rad);

// float comparison

bool eqf(const float& a, const float& b, const float& epsilon);
bool eqf(const float& a, const float& b); // default epsilon

float clamp(float n, float lower, float upper);

}

#endif /* INCLUDE_MATHUTILS_H_ */
