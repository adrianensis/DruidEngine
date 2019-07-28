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
	DE_CLASS(AnimationFrame);

	void init(const Vector2& position, f32 width, f32 height);

	const Vector2& getPosition() const;
	f32 getWidth() const;
	f32 getHeight() const;
};

} /* namespace DE */

#endif /* DE_ANIMATIONFRAME_H_ */
