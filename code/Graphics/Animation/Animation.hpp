#pragma once

#include "Core/DE_Class.hpp"

#include "Maths/Vector2.hpp"

namespace DE {

template<class T> class List;
class AnimationFrame;

class Animation: public DE_Class {

private:
	DE_M(Frames, List<AnimationFrame*>*);
	DE_M(CurrentFrameNumber, u32);
	DE_M(TimeAccumulator, f32);
	DE_M_SET(Speed, f32)

public:

	DE_CLASS_BODY(Animation)

	void init();

	/**
	 * Static method that creates an animation.
	 * /param frameCount The number of frames of the animation.
	 * /param horizontal True if horizontal (left to right), False if vertical (down to up).
	 * /param reverse True to use the reverse direction.
	 * /param startPosition The start position (bottom-left corner of the frame) in texture coordinates (see OpenGL texture coordinates).
	 * /param width The frame width.
	 * /param height The frame height.
	 * /param speed The speed of the animation.
	 * /returns The animation.
	 */
	static Animation* create(u32 frameCount, bool horizontal, bool reverse, const Vector2 &startPosition, f32 width,
			f32 height, f32 speed);

	void addFrame(AnimationFrame *frame);
	u32 getNumberOfFrames() const;
	const AnimationFrame* getNextFrame();
	const AnimationFrame* getCurrentFrame() const;
};

}

