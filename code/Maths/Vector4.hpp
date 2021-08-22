#pragma once

#include "Core/ObjectBase.hpp"

#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/VectorDef.hpp"

class Vector2;
class Vector3;

CLASS(Vector4, ObjectBase) /*16 bytes alignment*/
{
public:

	f32 x, y, z, w;

	Vector4();
	Vector4(f32 x, f32 y, f32 z, f32 w);
	Vector4(const Vector4 &other);
	Vector4(const Vector3 &other);
	Vector4(const Vector2 &other);
	Vector4(const Vector3 &other, f32 w);
	Vector4(const Vector2 &other, f32 z, f32 w);

	Vector4 &set(f32 x, f32 y, f32 z, f32 w);

	VECTOR_BASE_DEFINITION(4)
};