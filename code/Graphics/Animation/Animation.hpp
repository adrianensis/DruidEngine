#pragma once

#include "Core/Core.hpp"

#include "Maths/Vector2.hpp"


class AnimationFrame;

CLASS(Animation, ObjectBase)
{
	PRI(SVec(AnimationFrame *) *, Frames, NONE);
	PRI(u32, CurrentFrameNumber, NONE)
	PRI(f32, TimeAccumulator, NONE)
	PRI(f32, Speed, NONE)

public:
	Animation();
	~Animation() OVR;

	SET(Speed);

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
	STC Animation *create(u32 frameCount, bool horizontal, bool reverse, CNS Vector2 &startPosition, f32 width,
							 f32 height, f32 speed);

	void addFrame(AnimationFrame * frame);
	u32 getNumberOfFrames() CNS;
	CNS AnimationFrame *getNextFrame();
	CNS AnimationFrame *getCurrentFrame() CNS;
};