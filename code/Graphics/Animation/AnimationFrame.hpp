#pragma once

#include "Core/Core.hpp"

#include "Maths/Vector2.hpp"

class AnimationFrame: public ObjectBase
{
    GENERATE_METADATA(AnimationFrame)
	PRI_M(Vector2, Position, GET)
	PRI_M(f32, Width, GET)
	PRI_M(f32, Height, GET)

PUB
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