#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"


class Camera;

CLASS(Frustum, ObjectBase)
{
	PRI(VPmatrix, NONE, Matrix4)
	PRI(Camera, NONE, Camera *)
	PRI(Planes, NONE, SArr<Vector4, 6>);

public:
	void init(Camera * camera);

	/**
	* Builds the frustum planes.
	*/
	void build(bool forceBuild = false);

	bool testSphere(CNS Vector3 &center, f32 radius) CNS;
	bool testPoint(CNS Vector3 &point) CNS;
	bool testRectangle(CNS Vector3 &leftTop, f32 width, f32 height) CNS;
};