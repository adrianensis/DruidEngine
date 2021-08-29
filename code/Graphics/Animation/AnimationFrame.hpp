#pragma once

#include "Core/Core.hpp"

#include "Maths/Vector2.hpp"

CLASS(AnimationFrame, ObjectBase)
{
	PRI(Position, GET, Vector2)
	PRI(Width, GET, f32)
	PRI(Height, GET, f32)

public:
	void init(const Vector2 &position, f32 width, f32 height);
};