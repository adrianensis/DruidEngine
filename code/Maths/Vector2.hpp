#pragma once

#include "Core/Core.hpp"

#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/VectorDef.hpp"

class Vector3;
class Vector4;

CLASS(Vector2, ObjectBase)
{
public:

	f32 x, y;

	Vector2();
	Vector2(f32 x, f32 y);
	Vector2(CNS Vector2 &other);
	Vector2(CNS Vector3 &other);
    Vector2(CNS Vector4 &other);

	Vector2 &set(CNS Vector3 &other);
    Vector2 &set(CNS Vector4 &other);
	Vector2 &set(f32 x, f32 y);

	VECTOR_BASE_DEFINITION(2)
};
