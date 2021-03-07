#pragma once

#include "Core/DE_Class.hpp"

#include "Maths/Vector2.hpp"

namespace DE {

class AnimationFrame: public DE_Class {
private:

	DE_M_GET(Position, Vector2)
	DE_M_GET(Width, f32)
	DE_M_GET(Height, f32)

public:

	DE_CLASS_BODY(AnimationFrame)

	void init(const Vector2 &position, f32 width, f32 height);
};

}
