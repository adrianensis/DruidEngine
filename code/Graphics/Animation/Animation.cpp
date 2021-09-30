#include "Core/Time/TimeUtils.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Graphics/Animation/AnimationFrame.hpp"
#include "Log/Log.hpp"

Animation::Animation()
{
	mFrames = nullptr;
	mCurrentFrameNumber = 0;
	mSpeed = 1.0f;
	mTimeAccumulator = 0.0f;
}

Animation::~Animation()
{
	LIST_DELETE_CONTENT(*mFrames)

	DELETE(mFrames);
}

void Animation::init()
{
	mFrames = NEW(SVec(AnimationFrame *));
}

void Animation::addFrame(AnimationFrame *frame)
{
	mFrames->push_back(frame);
}
u32 Animation::getNumberOfFrames() CNS
{
	return mFrames->size();
}

Animation *Animation::create(u32 frameCount, bool horizontal, bool reverse, CNS Vector2 &startPosition, f32 width,
							 f32 height, f32 speed)
{
	// TODO: check if coordinates are > 1 and < 0 !!!!!

	Animation *animation = NEW(Animation);
	animation->init();
	animation->setSpeed(speed);

	i32 horizontalDir = 0;
	i32 verticalDir = 0;

	if (horizontal)
		horizontalDir = 1;
	else
		verticalDir = 1;

	if (reverse)
	{
		horizontalDir *= -1;
		verticalDir *= -1;
	}

	i32 start = 0;
	i32 end = frameCount;
	i32 delta = 1;

	if (reverse)
	{
		start = frameCount - 1;
		end = 0;
		delta = -1;
	}

	for (i32 i = start; i != end; i += delta)
	{
		Vector2 pos(0, 0);
		pos.add(startPosition);

		if (horizontalDir != 0)
			pos.x += i * width;

		if (verticalDir != 0)
			pos.y += i * height;

		AnimationFrame *frame = NEW(AnimationFrame);
		frame->init(pos, width, height);
		animation->addFrame(frame);
	}

	return animation;
}

CNS AnimationFrame *Animation::getNextFrame()
{
	CNS AnimationFrame *frame = nullptr;

	if (mFrames->size() > 0)
	{
		// speed -> frame/second.
		// time -> time of one frame.

		f32 time = (1.0 / (mSpeed)) * 1000.0f; // in milliseconds !

		mTimeAccumulator += Time::getInstance()->getDeltaTimeMillis();

		//ECHO("RENDERER NUM")

		// if delta time is greater than 'one frame time'
		// then -> change to the next frame.
		if (mTimeAccumulator >= time)
		{
			mTimeAccumulator = 0.0f;
			mCurrentFrameNumber = (mCurrentFrameNumber + 1) % mFrames->size();
		}

		frame = mFrames->at(mCurrentFrameNumber);
	}

	return frame;
}

CNS AnimationFrame *Animation::getCurrentFrame() CNS
{
	return mFrames->at(mCurrentFrameNumber);
}