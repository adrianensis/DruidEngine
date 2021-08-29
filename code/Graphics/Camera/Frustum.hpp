#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"

#include <array>

class Camera;

CLASS(Frustum, ObjectBase)
{
	PRI(VPmatrix, NONE, Matrix4)
	PRI(Camera, NONE, Camera *)
	PRI(Planes, NONE, std::array<Vector4, 6>);

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