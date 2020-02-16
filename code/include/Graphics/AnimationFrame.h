#ifndef DE_ANIMATIONFRAME_H_
#define DE_ANIMATIONFRAME_H_

#include "DE_Class.h"

#include "Vector2.h"

namespace DE {

class AnimationFrame : public DE_Class{

private:
	Vector2 mPosition;
	f32 mWidth;
	f32 mHeight;

public:
	DE_CLASS(AnimationFrame, DE_Class);

	void init(const Vector2& position, f32 width, f32 height);

	const Vector2& getPosition() const { return mPosition; };
	f32 getWidth() const { return mWidth; };
	f32 getHeight() const { return mHeight; };
};

} /* namespace DE */

#endif /* DE_ANIMATIONFRAME_H_ */
