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

	DE_GET(const Vector2&, Position);
	DE_GET(f32, Width);
	DE_GET(f32, Height);
};

} /* namespace DE */

#endif /* DE_ANIMATIONFRAME_H_ */
