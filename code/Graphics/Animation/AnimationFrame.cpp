#include "Graphics/Animation/AnimationFrame.hpp"
#include "Log/Log.hpp"

void AnimationFrame::init(const Vector2 &position, f32 width, f32 height)
{
	mPosition = position;
	mWidth = width;
	mHeight = height;
}