#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"

class Camera;

class Frustum: public ObjectBase
{
    GENERATE_METADATA(Frustum)
	PRI Matrix4 mVPmatrix;
	PRI Camera* mCamera = nullptr;
	PRI std::array<Vector4, 6> mPlanes;

PUB
	void init(Camera * camera);

	/**
	* Builds the frustum planes.
	*/
	void build();

	bool testSphere(const Vector3 &center, f32 radius) const;
	bool testPoint(const Vector3 &point) const;
	bool testRectangle(const Vector3 &leftTop, f32 width, f32 height) const;
};