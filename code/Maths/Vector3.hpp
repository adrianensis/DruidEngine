#pragma once

#include "Core/ObjectBase.hpp"

#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/VectorDef.hpp"

class Vector2;
class Vector4;

CLASS(Vector3, ObjectBase)
{
public:

	f32 x, y, z;

	Vector3();
	Vector3(f32 x, f32 y, f32 z);
	Vector3(const Vector3 &other);
	Vector3(const Vector2 &other);
	Vector3(const Vector2 &other, f32 z);
	Vector3(const Vector4 &other);

	Vector3 &set(f32 x, f32 y, f32 z);
	Vector3 &cross(const Vector3 &v);
	Vector3 &nlerp(const Vector3 &target, f32 t);
	Vector3 &slerp(const Vector3 &target, f32 t);
	f32 angle(const Vector3 &v, const Vector3 &n) const;

	VECTOR_BASE_DEFINITION(3)
};
