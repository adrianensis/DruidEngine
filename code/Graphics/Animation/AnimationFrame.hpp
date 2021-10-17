#pragma once

#include "Core/Core.hpp"

#include "Maths/Vector2.hpp"

CLASS(AnimationFrame, ObjectBase)
{
	PRI(Vector2, Position, GET)
	PRI(f32, Width, GET)
	PRI(f32, Height, GET)

public:
	void init(const Vector2 &position, f32 width, f32 height);

    COPY(AnimationFrame)
    {
        DO_COPY(Position)
        DO_COPY(Width)
        DO_COPY(Height)
    }

    SERIALIZE();
    DESERIALIZE();
};