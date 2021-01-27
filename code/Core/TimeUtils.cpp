#include "Core/TimeUtils.hpp"
#include "Log/Log.hpp"

namespace DE {

TimeMark::TimeMark() : DE_Class(){
	mDeltaTimeMillis = 0.0;
	mDeltaTimeSeconds = 0.0;
}

TimeMark::~TimeMark() = default;

void TimeMark::init() {
	mDeltaTimeMillis = 0.0;
	mDeltaTimeSeconds = 0.0;
	mLastTime = std::chrono::high_resolution_clock::now();
}

void TimeMark::startFrame() {
	mStartTime = std::chrono::high_resolution_clock::now();
}

void TimeMark::endFrame() {
	mLastTime = std::chrono::high_resolution_clock::now();
	mDeltaTimeChronoDuration = std::chrono::duration_cast<std::chrono::milliseconds>(mLastTime - mStartTime);
	mDeltaTimeMillis = mDeltaTimeChronoDuration.count();
	mDeltaTimeSeconds = mDeltaTimeMillis / 1000.0f;
}

f32 TimeMark::getElapsedTimeMillis() {
	auto now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>( now - mStartTime ).count();
}

f32 TimeMark::getDeltaTimeMillis() {
	return mDeltaTimeMillis;
}

f32 TimeMark::getDeltaTimeSeconds() {
	return mDeltaTimeSeconds;
}

Time::Time() : DE_Class(), Singleton<Time>(){

}

Time::~Time() = default;

}
