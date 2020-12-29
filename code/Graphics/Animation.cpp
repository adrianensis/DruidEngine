#include <TimeUtils.hpp>
#include "Animation.hpp"

#include "Memory.hpp"
#include "AnimationFrame.hpp"
#include "List.hpp"
#include "Log.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Animation::Animation() : DE_Class() {
	mFrames = nullptr;
	mCurrentFrameNumber = 0;
	mSpeed = 1.0f;
	mTimeAccumulator = 0.0f;
}

Animation::~Animation() {
	FOR_LIST(it, mFrames){
		Memory::free<AnimationFrame>(it.get());
	}

	Memory::free<List<AnimationFrame*>>(mFrames);
}

void Animation::init() {
	DE_TRACE()

	mFrames = Memory::allocate<List<AnimationFrame*>>();
	mFrames->init();
}

// ---------------------------------------------------------------------------

void Animation::addFrame(AnimationFrame *frame) {
	mFrames->pushBack(frame);
}
u32 Animation::getNumberOfFrames() const {
	return mFrames->getLength();
}

// ---------------------------------------------------------------------------

Animation* Animation::create(u32 frameCount, bool horizontal, bool reverse, const Vector2 &startPosition, f32 width,
		f32 height, f32 speed) {

	// TODO: check if coordinates are > 1 or < 0 !!!!!

	Animation* animation = Memory::allocate<Animation>();
	animation->init();
	animation->setSpeed(speed);

	i32 horizontalDir = 0;
	i32 verticalDir = 0;

	if (horizontal)
		horizontalDir = 1;
	else
		verticalDir = 1;

	if (reverse) {
		horizontalDir *= -1;
		verticalDir *= -1;
	}

	i32 start = 0;
	i32 end = frameCount;
	i32 delta = 1;

	if (reverse) {
		start = frameCount - 1;
		end = 0;
		delta = -1;
	}

	for (u32 i = start; i != end; i += delta) {

		Vector2 pos(0, 0);
		pos.add(startPosition);

		if (horizontalDir != 0)
			pos.x += i * width;

		if (verticalDir != 0)
			pos.y += i * height;

		AnimationFrame* frame = Memory::allocate<AnimationFrame>();
		frame->init(pos, width, height);
		animation->addFrame(frame);

	}

	return animation;
}

const AnimationFrame* Animation::getNextFrame() {

	const AnimationFrame* frame = nullptr;

	if (mFrames->getLength() > 0) {
		// speed -> frame/second.
		// time -> time of one frame.

		f32 time = (1.0 / (mSpeed)) * 1000.0f; // in milliseconds !

		mTimeAccumulator += Time::getInstance()->getDeltaTimeMillis();

		//ECHO("RENDERER NUM")
		//VAL(f32,mTimeAccumulator)

		// if delta time is greater than 'one frame time'
		// then -> change to the next frame.
		if (mTimeAccumulator >= time) {
			mTimeAccumulator = 0.0f;
			mCurrentFrameNumber = (mCurrentFrameNumber + 1) % mFrames->getLength();
		}

		frame = mFrames->get(mCurrentFrameNumber);
	}

	return frame;
}

const AnimationFrame* Animation::getCurrentFrame() const {
	return mFrames->get(mCurrentFrameNumber);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
