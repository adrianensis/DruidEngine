#include "Core/Time/TimeUtils.hpp"

void TimeMark::init()
{
	mIsStarted = false;
	mDeltaTimeMillis = 0.0;
	mLastTime = std::chrono::high_resolution_clock::now();
}

void TimeMark::start()
{
	mIsStarted = true;
	mStartTime = std::chrono::high_resolution_clock::now();
}

void TimeMark::end()
{
	if (mIsStarted)
	{
		mIsStarted = false;
		mLastTime = std::chrono::high_resolution_clock::now();
		mDeltaTimeChronoDuration = std::chrono::duration_cast<std::chrono::milliseconds>(mLastTime - mStartTime);
		mDeltaTimeMillis = mDeltaTimeChronoDuration.count();
	}
}

f32 TimeMark::getElapsedTimeMillis()
{
	auto now = std::chrono::high_resolution_clock::now();
	return mIsStarted ? std::chrono::duration_cast<std::chrono::milliseconds>(now - mStartTime).count() : 0.0f;
}

f32 TimeMark::getElapsedTimeSeconds()
{
	return getElapsedTimeMillis() / 1000.0f;
}

f32 TimeMark::getDeltaTimeMillis()
{
	return mDeltaTimeMillis;
}

f32 TimeMark::getDeltaTimeSeconds()
{
	return getDeltaTimeMillis() / 1000.0f;
}