#include "Graphics/Animation/AnimationFrame.hpp"
#include "Log/Log.hpp"

namespace DE {

AnimationFrame::AnimationFrame() : ObjectBase() {
	mWidth = 0.0f;
	mHeight = 0.0f;
}

AnimationFrame::~AnimationFrame() {

}

void AnimationFrame::init(const Vector2 &position, f32 width, f32 height) {
	mPosition = position;
	mWidth = width;
	mHeight = height;
}

}
