#ifndef DE_ANIMATIONFRAME_H_
#define DE_ANIMATIONFRAME_H_

#include "Core/DE_Class.hpp"

#include "Maths/Vector2.hpp"

namespace DE {

class AnimationFrame: public DE_Class {
private:

	DE_M_GET(Position, Vector2)
	DE_M_GET(Width, f32)
	DE_M_GET(Height, f32)

public:

	DE_CLASS(AnimationFrame)

	void init(const Vector2 &position, f32 width, f32 height);
};

} /* namespace DE */

#endif /* DE_ANIMATIONFRAME_H_ */
