#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Graphics/Animation/AnimationFrame.hpp"

class Animation: public ObjectBase
{
    GENERATE_METADATA(Animation)
    PRI std::string mName; GETREF_CONST_SET(Name)
	PRI std::vector<AnimationFrame> mFrames; GETREF_CONST(Frames);
	PRI u32 mCurrentFrameNumber = 0;
	PRI f32 mTimeAccumulator = 0.0f;
	PRI f32 mSpeed = 0.0f; GET_SET(Speed)

PUB
	Animation();
	~Animation() override;

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