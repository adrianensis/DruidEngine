#pragma once

#include "Core/ObjectBase.hpp"

#include "Maths/Vector2.hpp"

namespace DE {

class AnimationFrame : public ObjectBase {
private:

	 Vector2 mPosition;
	 f32 mWidth;
	 f32 mHeight;

public:

	GENERATE_METADATA(AnimationFrame);

	AnimationFrame();
	virtual ~AnimationFrame() override;;

	GET(Position);
	GET(Width);
	GET(Height);

	void init(const Vector2 &position, f32 width, f32 height);
};
}

