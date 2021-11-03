#pragma once

#include "Core/Core.hpp"

#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/VectorDef.hpp"

class Vector2;
class Vector3;

class Vector4: public ObjectBase
{
	GENERATE_METADATA(Vector4)
PUB

	f32 x, y, z, w;

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