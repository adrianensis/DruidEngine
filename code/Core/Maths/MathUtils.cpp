#include "Core/Maths/MathUtils.hpp"
#include <algorithm>

f32 MathUtils::FLOAT_EPSILON = 0.0000001f;

f32 MathUtils::PI = acosf(-1.0f);			   // pi
f32 MathUtils::PI_2 = PI / 2.0f;			   // pi/2
f32 MathUtils::PI_4 = PI / 4.0f;			   // pi/4
f32 MathUtils::INV_PI = 1.0f / PI;			   // 1/pi
f32 MathUtils::INV_2_PI = 2.0f / PI;		   // 2/pi
f32 MathUtils::INV_2_SQRTPI = 2 / sqrtf(PI);   // 2/sqrt(pi)
f32 MathUtils::SQRT2 = sqrtf(2.0f);			   // sqrt(2)
f32 MathUtils::INV_SQRT2 = 1.0f / sqrtf(2.0f); // 1/sqrt(2)
f32 MathUtils::E = 2.71828182845904523536;	   // e
f32 MathUtils::LOG2E = log2(E);				   // log_2(e)
f32 MathUtils::LOG10E = log10(E);			   // log_10(e)
f32 MathUtils::LN2 = log1p(2.0f);			   // log_e(2)
f32 MathUtils::LN10 = log1p(10.0f);			   // log_e(10)

f32 MathUtils::PI_180 = PI / 180.0f;
f32 MathUtils::INV_PI_180 = 180.0f / PI;

f32 MathUtils::rad(f32 deg)
{
	return deg * PI_180;
}

f32 MathUtils::deg(f32 rad)
{
	return rad * INV_PI_180;
}

bool MathUtils::eqf(f32 a, f32 b, f32 epsilon)
{
	return fabsf(a - b) < epsilon;
}

bool MathUtils::eqf(f32 a, f32 b)
{
	return eqf(a, b, FLOAT_EPSILON);
}

f32 MathUtils::clamp(f32 n, f32 lower, f32 upper)
{
	return std::max(lower, std::min(n, upper));
}