#include "Graphics/Animation/AnimationFrame.hpp"

void AnimationFrame::init(const Vector2 &position, f32 width, f32 height)
{
	mPosition = position;
	mWidth = width;
	mHeight = height;
}

void AnimationFrame::serialize(JSON &json) const
{
    DO_SERIALIZE("position", mPosition);
    DO_SERIALIZE("width", mWidth);
    DO_SERIALIZE("height", mHeight);
}

void AnimationFrame::deserialize(const JSON &json)
{
    DO_DESERIALIZE("position", mPosition);
    DO_DESERIALIZE("width", mWidth);
    DO_DESERIALIZE("height", mHeight);
}