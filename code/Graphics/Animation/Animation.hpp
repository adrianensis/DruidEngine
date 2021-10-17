#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Graphics/Animation/AnimationFrame.hpp"

CLASS(Animation, ObjectBase)
{
    PRI(SStr, Name, GETREF_CONST_SET)
	PRI(SVec(AnimationFrame), Frames, GETREF_CONST);
	PRI(u32, CurrentFrameNumber, NONE)
	PRI(f32, TimeAccumulator, NONE)
	PRI(f32, Speed, GET_SET)

public:
	Animation();
	~Animation() OVR;

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
	static Animation create(u32 frameCount, bool horizontal, bool reverse, const Vector2 &startPosition, f32 width,
							 f32 height, f32 speed);

	u32 getNumberOfFrames() const;
	const AnimationFrame &getNextFrame();
	const AnimationFrame &getCurrentFrame() const;

    COPY(Animation)
    {
        DO_COPY(Speed);

        FOR_LIST(it, other->mFrames)
        {
            this->mFrames.push_back(*it);
        }
    }

    SERIALIZE();
    DESERIALIZE();
};