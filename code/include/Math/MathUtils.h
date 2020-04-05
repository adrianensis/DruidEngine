#ifndef DE_MATHUTILS_H
#define DE_MATHUTILS_H

#include <cmath> /* ceil, max */
#include "BasicTypes.h"
#include "Vector2.h"
#include "Vector3.h"
#include <limits>

namespace DE {

class MathUtils {
public:

	static f32 FLOAT_EPSILON;

	static f32 PI;
	static f32 PI_2;
	static f32 PI_4;
	static f32 INV_PI;
	static f32 INV_2_PI;
	static f32 INV_2_SQRTPI;
	static f32 SQRT2;
	static f32 INV_SQRT2;
	static f32 E;
	static f32 LOG2E;
	static f32 LOG10E;
	static f32 LN2;
	static f32 LN10;

	static f32 PI_180;
	static f32 INV_PI_180;

	static f32 rad(f32 deg);
	static f32 deg(f32 rad);

	// f32 comparison

	static bool eqf(f32 a, f32 b, f32 epsilon);
	static bool eqf(f32 a, f32 b); // default epsilon

	static f32 clamp(f32 n, f32 lower, f32 upper);

	// Geometry tests

	static bool testRectanglePoint(const Vector2& leftTop, f32 width, f32 height, const Vector2& point, f32 eps);
	static bool testSphereSphere(const Vector2& centerA, const Vector2& centerB, f32 radiusA, f32 radiusB);
};
}

#endif /* DE_MATHUTILS_H */
