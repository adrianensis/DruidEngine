#pragma once

#include "Core/Core.hpp"

#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/VectorDef.hpp"

class Vector3;

CLASS(Vector2, ObjectBase)
{
public:

	f32 x, y;

	Vector2();
	Vector2(f32 x, f32 y);
	Vector2(const Vector2 &other);
	Vector2(const Vector3 &other);

	Vector2 &set(f32 x, f32 y);

	VECTOR_BASE_DEFINITION(2)
};
