#pragma once

#include "Core/Core.hpp"

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
	Vector3(CNS Vector3 &other);
	Vector3(CNS Vector2 &other);
	Vector3(CNS Vector2 &other, f32 z);
	Vector3(CNS Vector4 &other);

	Vector3 &set(f32 x, f32 y, f32 z);
    Vector3 &set(CNS Vector2 &other);
    Vector3 &set(CNS Vector4 &other);

	Vector3 &cross(CNS Vector3 &v);
	Vector3 &nlerp(CNS Vector3 &target, f32 t);
	Vector3 &slerp(CNS Vector3 &target, f32 t);
	f32 angle(CNS Vector3 &v, CNS Vector3 &n) CNS;

	VECTOR_BASE_DEFINITION(3)
};
