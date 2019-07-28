#include "AnimationFrame.h"

namespace DE {

// ---------------------------------------------------------------------------

AnimationFrame::AnimationFrame() : DE_Class(){
  mWidth = 0.0f;
  mHeight = 0.0f;
}

AnimationFrame::~AnimationFrame() {

}

void AnimationFrame::init(const Vector2& position, f32 width, f32 height) {
    mPosition = position;
    mWidth = width;
    mHeight = height;
}

const Vector2& AnimationFrame::getPosition() const { return mPosition; }
f32 AnimationFrame::getWidth() const { return mWidth; }
f32 AnimationFrame::getHeight() const { return mHeight; }

// ---------------------------------------------------------------------------

} /* namespace DE */
