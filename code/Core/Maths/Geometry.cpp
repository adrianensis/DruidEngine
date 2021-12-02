#include "Core/Maths/Geometry.hpp"

#include "Core/Maths/MathUtils.hpp"

bool Geometry::testRectanglePoint(const Vector2 &leftTop, f32 width, f32 height, const Vector2 &point, f32 eps)
{
	return (leftTop.x - eps <= point.x && leftTop.y + eps >= point.y && leftTop.x + width + eps >= point.x && leftTop.y - height - eps <= point.y);
}

bool Geometry::testRectangleSphere(const Vector2 &leftTop, f32 width, f32 height, const Vector2 &center, f32 radius, f32 eps)
{
	return (leftTop.x - radius - eps <= center.x && leftTop.y + radius + eps >= center.y && leftTop.x + width + radius + eps >= center.x && leftTop.y - height - radius - eps <= center.y);
}

bool Geometry::testSphereSphere(const Vector2 &centerA, const Vector2 &centerB, f32 radiusA, f32 radiusB, f32 eps)
{
	f32 distance = centerA.dst(centerB);
	return (distance < (radiusA + radiusB + eps));
}

bool testLineLine(const Vector2 &lineAStart, const Vector2 &lineAEnd, const Vector2 &lineBStart,
				  const Vector2 &lineBEnd, Vector2 &intersectionResult)
{
	// Source : http://www-cs.ccny.cuny.edu/~wolberg/capstone/intersection/Intersection%20point%20of%20two%20lines.html

	/*
	 Pa = P1 + ua ( P2 - P1 )
	 Pb = P3 + ub ( P4 - P3 )

	 Pa = Pb
	 */

	// calculate the distance to intersection point
	f32 uA = ((lineBEnd.x - lineBStart.x) * (lineAStart.y - lineBStart.y) - (lineBEnd.y - lineBStart.y) * (lineAStart.x - lineBStart.x)) / ((lineBEnd.y - lineBStart.y) * (lineAEnd.x - lineAStart.x) - (lineBEnd.x - lineBStart.x) * (lineAEnd.y - lineAStart.y));
	f32 uB = ((lineAEnd.x - lineAStart.x) * (lineAStart.y - lineBStart.y) - (lineAEnd.y - lineAStart.y) * (lineAStart.x - lineBStart.x)) / ((lineBEnd.y - lineBStart.y) * (lineAEnd.x - lineAStart.x) - (lineBEnd.x - lineBStart.x) * (lineAEnd.y - lineAStart.y));

	bool intersection = false;
	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		intersectionResult.x = lineAStart.x + (uA * (lineAEnd.x - lineAStart.x));
		intersectionResult.y = lineAStart.y + (uA * (lineAEnd.y - lineAStart.y));

		intersection = true;
	}

	return intersection;
}

bool Geometry::testLineSphereSimple(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &center,
									f32 radius, f32 eps)
{
	bool lineIntersectsSphere = false;

	Vector2 closestPoint(closestPointInLine(lineStart, lineEnd, center));

	if (testSpherePoint(lineStart, center, radius + eps) || testSpherePoint(lineEnd, center, radius + eps))
	{
		lineIntersectsSphere = true;
	}
	else
	{
		lineIntersectsSphere = testSpherePoint(closestPoint, center, radius + eps);
	}

	return lineIntersectsSphere;
}

bool Geometry::testLineSphere(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &center, f32 radius,
							  f32 eps, Vector2 &intersectionResult1, Vector2 &intersectionResult2)
{
	// X(t) = x1 + (x2 - x1) * t
	// Y(t) = y1 + (y2 - y1) * t
	//
	// (X - center.x)2 + (Y - center.y)2 = radius2

	f32 radiusEps = radius + eps;

	bool lineIntersectsSphere = false;

	f32 t;

	f32 dx = lineEnd.x - lineStart.x;
	f32 dy = lineEnd.y - lineStart.y;

	Vector2 startToCenter = lineStart - center;

	f32 A = dx * dx + dy * dy;
	f32 B = 2.0f * (dx * startToCenter.x + dy * startToCenter.y);
	f32 C = startToCenter.x * startToCenter.x + startToCenter.y * startToCenter.y - radiusEps * radiusEps;

	f32 det = B * B - 4 * A * C;

	if (det >= 0)
	{
		// Two solutions.
		f32 sqrtDet = sqrtf(det);

		t = (f32)((-B + sqrtDet) / (2 * A));
		intersectionResult1 = Vector2(lineStart.x + t * dx, lineStart.y + t * dy);

		t = (f32)((-B - sqrtDet) / (2 * A));
		intersectionResult2 = Vector2(lineStart.x + t * dx, lineStart.y + t * dy);

		lineIntersectsSphere = true;
	}

	return lineIntersectsSphere;
}

bool Geometry::testSpherePoint(const Vector2 &point, const Vector2 &center, f32 radius)
{
	return center.dst(point) <= radius;
}

bool Geometry::testLinePoint(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &point, f32 eps)
{
	// get distance from the point to the two ends of the line
	f32 d1 = lineStart.dst(point);
	f32 d2 = lineEnd.dst(point);

	f32 lineLen = lineStart.dst(lineEnd);

	bool pointIsInLine = false;

	if (d1 + d2 >= lineLen - eps && d1 + d2 <= lineLen + eps)
	{
		pointIsInLine = true;
	}

	return pointIsInLine;
}

Vector2 Geometry::closestPointInLine(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &point)
{
	Vector2 pointStartVector = (point - lineStart) /*.nor()*/;
	Vector2 lineVector = (lineEnd - lineStart) /*.nor()*/;

	f32 t = pointStartVector.dot(lineVector) / lineVector.dot(lineVector);

	t = std::fmaxf(t, 0.0f); // clamp to 0
	t = std::fminf(t, 1.0f); // clampt to 1

	return Vector2(lineStart + (lineVector.mul(t)));
}

Vector2 Geometry::midPoint(const Vector2 &a, const Vector2 &b)
{
	return Vector2((a.x + b.x) / 2.0f, (a.y + b.y) / 2.0f);
};
