#ifndef DE_ANIMATIONFRAME_H_
#define DE_ANIMATIONFRAME_H_

#include "DE_Class.hpp"

#include "Vector2.hpp"

namespace DE {

class AnimationFrame: public DE_Class {
private:

	Vector2 mPosition;
	f32 mWidth;
	f32 mHeight;

public:

	DE_CLASS(AnimationFrame, DE_Class)

	DE_GET(Position)
	DE_GET(Width)
	DE_GET(Height)

	void init(const Vector2 &position, f32 width, f32 height);
};

} /* namespace DE */

#endif /* DE_ANIMATIONFRAME_H_ */
