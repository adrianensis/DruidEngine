#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Maths/VectorDef.hpp"
#include "Core/Maths/VectorDef.hpp"

class Vector3;
class Vector4;

class Vector2: public ObjectBase
{
    GENERATE_METADATA(Vector2)
PUB

	f32 x, y;

	Vector2();
	Vector2(f32 x, f32 y);
	Vector2(const Vector2 &other);
	Vector2(const Vector3 &other);
    Vector2(const Vector4 &other);

	Vector2 &set(const Vector3 &other);
    Vector2 &set(const Vector4 &other);
	Vector2 &set(f32 x, f32 y);

	VECTOR_BASE_DEFINITION(2)
};