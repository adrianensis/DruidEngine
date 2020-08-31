#include <TimeUtils.hpp>
#include "Log.hpp"

namespace DE {


// ---------------------------------------------------------------------------

Time::Time() : DE_Class(), Singleton<Time>(){

}

Time::~Time() = default;

// ---------------------------------------------------------------------------

void Time::init() {
	TRACE();

	mDeltaTimeMillis = 0.0;
	mDeltaTimeSeconds = 0.0;
	mLastTime = std::chrono::high_resolution_clock::now();

	mInitializationTime = std::chrono::high_resolution_clock::now();
}

// ---------------------------------------------------------------------------

void Time::startFrame() {
	mStartTime = std::chrono::high_resolution_clock::now();
}

// ---------------------------------------------------------------------------

void Time::endFrame() {
	mLastTime = std::chrono::high_resolution_clock::now();
	mDeltaTimeChronoDuration = std::chrono::duration_cast<std::chrono::milliseconds>(mLastTime - mStartTime);
	mDeltaTimeMillis = mDeltaTimeChronoDuration.count();
	mDeltaTimeSeconds = mDeltaTimeMillis / 1000.0f;
}

// ---------------------------------------------------------------------------

f32 Time::getElapsedTime() {
	auto now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>( now - mStartTime ).count();
}

// ---------------------------------------------------------------------------

f32 Time::getDeltaTimeMillis() {
	return mDeltaTimeMillis;
}

// ---------------------------------------------------------------------------

f32 Time::getDeltaTimeSeconds() {
	return mDeltaTimeSeconds;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
