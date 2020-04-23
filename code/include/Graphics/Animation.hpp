#ifndef DE_ANIMATION_H_
#define DE_ANIMATION_H_

#include "DE_Class.hpp"

#include "Vector2.hpp"

namespace DE {

	template <class T> class List;
	class AnimationFrame;

class Animation : public DE_Class{

private:
	List<AnimationFrame*>* mFrames;
	u32 mCurrentFrameNumber;
	f32 mSpeed;
	f32 mTimeAccumulator;

public:
	DE_CLASS(Animation, DE_Class);

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
	static Animation* create(u32 frameCount, bool horizontal, bool reverse, const Vector2& startPosition, f32 width, f32 height, f32 speed);

	void init();

	void addFrame(AnimationFrame* frame);

	u32 getNumberOfFrames() const;
	const AnimationFrame* getNextFrame();

	void setSpeed(f32 newSpeed ){ mSpeed = newSpeed; };
	u32 getCurrentFrameNumber() const { return mCurrentFrameNumber; };
};

} /* namespace DE */

#endif /* DE_ANIMATION_H_ */
