#include "Graphics/Animation/AnimationFrame.hpp"
#include "Log/Log.hpp"

void AnimationFrame::init(const Vector2 &position, f32 width, f32 height)
{
	mPosition = position;
	mWidth = width;
	mHeight = height;
}

SERIALIZE_IMPL(AnimationFrame)
{
    DO_SERIALIZE("position", mPosition);
    DO_SERIALIZE("width", mWidth);
    DO_SERIALIZE("height", mHeight);
}

DESERIALIZE_IMPL(AnimationFrame)
{
    DO_DESERIALIZE("position", mPosition);
    DO_DESERIALIZE("width", mWidth);
    DO_DESERIALIZE("height", mHeight);
}