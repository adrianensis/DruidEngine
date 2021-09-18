#include "Graphics/Animation/AnimationFrame.hpp"
#include "Log/Log.hpp"

void AnimationFrame::init(CNS Vector2 &position, f32 width, f32 height)
{
	mPosition = position;
	mWidth = width;
	mHeight = height;
}