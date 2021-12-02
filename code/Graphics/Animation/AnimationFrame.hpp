#pragma once

#include "Core/Core.hpp"
#include "Maths/Maths.hpp"

class AnimationFrame: public ObjectBase
{
    GENERATE_METADATA(AnimationFrame)
	PRI Vector2 mPosition; GET(Position)
	PRI f32 mWidth = 0.0f; GET(Width)
	PRI f32 mHeight = 0.0f; GET(Height)

PUB
	void init(const Vector2 &position, f32 width, f32 height);

    COPY(AnimationFrame)
    {
        DO_COPY(mPosition)
        DO_COPY(mWidth)
        DO_COPY(mHeight)
    }

    virtual void serialize(JSON &json) const override;
    virtual void deserialize(const JSON &json) override;
};