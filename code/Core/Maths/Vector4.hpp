#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/Serialization.hpp"
#include "Core/Maths/VectorDef.hpp"

class Vector2;
class Vector3;

class Vector4
{
PUB

	f32 x = 0;
	f32 y = 0;
	f32 z = 0;
	f32 w = 0;

	Vector4();
	Vector4(f32 x, f32 y, f32 z, f32 w);
	Vector4(const Vector4 &other);
	Vector4(const Vector3 &other);
	Vector4(const Vector2 &other);
	Vector4(const Vector3 &other, f32 w);
	Vector4(const Vector2 &other, f32 z, f32 w);

	Vector4 &set(f32 x, f32 y, f32 z, f32 w);
    Vector4 &set(const Vector2 &other);
	Vector4 &set(const Vector3 &other);

	VECTOR_BASE_DEFINITION(4)
};

TEMPLATED_SERIALIZATION(Vector4)