#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"


class Camera;

CLASS(Frustum, ObjectBase)
{
	PRI(Matrix4, VPmatrix, NONE)
	PRI(Camera *, Camera, NONE)
	PRI(SArr(Vector4, 6), Planes, NONE);

public:
	void init(Camera * camera);

	/**
	* Builds the frustum planes.
	*/
	void build(bool forceBuild = false);

	bool testSphere(const Vector3 &center, f32 radius) const;
	bool testPoint(const Vector3 &point) const;
	bool testRectangle(const Vector3 &leftTop, f32 width, f32 height) const;
};