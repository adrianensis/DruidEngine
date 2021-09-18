#pragma once

#include <cmath> /* ceil, max */
#include <algorithm>
#include "Core/BasicTypes.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Geometry.hpp"
#include <limits>

class MathUtils
{
public:
	STC f32 FLOAT_EPSILON;

	STC f32 PI;
	STC f32 PI_2;
	STC f32 PI_4;
	STC f32 INV_PI;
	STC f32 INV_2_PI;
	STC f32 INV_2_SQRTPI;
	STC f32 SQRT2;
	STC f32 INV_SQRT2;
	STC f32 E;
	STC f32 LOG2E;
	STC f32 LOG10E;
	STC f32 LN2;
	STC f32 LN10;

	STC f32 PI_180;
	STC f32 INV_PI_180;

	STC f32 rad(f32 deg);
	STC f32 deg(f32 rad);

	// f32 comparison

	STC bool eqf(f32 a, f32 b, f32 epsilon);
	STC bool eqf(f32 a, f32 b); // default epsilon

	STC f32 clamp(f32 n, f32 lower, f32 upper);
};
