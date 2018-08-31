#ifndef DE_MATHUTILS_H
#define DE_MATHUTILS_H

#include <cmath> /* ceil, max */
#include "BasicTypes.h"

namespace DE {

class MathUtils {
public:

	static const f32 FLOAT_EPSILON;

	static const f32 PI;
	static const f32 PI_2;
	static const f32 PI_4;
	static const f32 INV_PI;
	static const f32 INV_2_PI;
	static const f32 INV_2_SQRTPI;
	static const f32 SQRT2;
	static const f32 INV_SQRT2;
	static const f32 E;
	static const f32 LOG2E;
	static const f32 LOG10E;
	static const f32 LN2;
	static const f32 LN10;

	static const f32 PI_180;
	static const f32 INV_PI_180;

	static f32 rad(const f32 deg);
	static f32 deg(const f32 rad);

	// f32 comparison

	static bool eqf(const f32 a, const f32 b, const f32 epsilon);
	static bool eqf(const f32 a, const f32 b); // default epsilon

	static f32 clamp(const f32 n, const f32 lower, const f32 upper);

};
}

#endif /* DE_MATHUTILS_H */
