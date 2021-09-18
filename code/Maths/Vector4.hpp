#pragma once

#include "Core/Core.hpp"

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
	Vector4(CNS Vector4 &other);
	Vector4(CNS Vector3 &other);
	Vector4(CNS Vector2 &other);
	Vector4(CNS Vector3 &other, f32 w);
	Vector4(CNS Vector2 &other, f32 z, f32 w);

	Vector4 &set(f32 x, f32 y, f32 z, f32 w);
    Vector4 &set(CNS Vector2 &other);
	Vector4 &set(CNS Vector3 &other);

	VECTOR_BASE_DEFINITION(4)
};