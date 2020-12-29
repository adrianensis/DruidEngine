#ifndef DE_TIME_H
#define DE_TIME_H

#include "BasicTypes.hpp"
#include "Singleton.hpp"
#include "DE_Class.hpp"
#include "Log.hpp"
#include <chrono>

namespace DE {

class TimeMark : public DE_Class {
private:
	f32 mDeltaTimeMillis = 0.0;
	f32 mDeltaTimeSeconds = 0.0;
	std::chrono::milliseconds mDeltaTimeChronoDuration;
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime, mLastTime;
public:
	DE_CLASS(TimeMark, DE_Class)

	void init();
	void startFrame();
	void endFrame();
	f32 getElapsedTime();
	f32 getDeltaTimeMillis();
	f32 getDeltaTimeSeconds();

	TimeMark& operator= (const TimeMark &other) {
		// self-assignment guard
		if (this == &other)
			return *this;

		// do the copy
		mDeltaTimeMillis = other.mDeltaTimeMillis;
		mDeltaTimeSeconds = other.mDeltaTimeSeconds;
		mDeltaTimeChronoDuration = other.mDeltaTimeChronoDuration;
		mStartTime = other.mStartTime;
		mLastTime = other.mLastTime;

		return *this;
	}
};

class Time : public TimeMark, public Singleton<Time>{
private:
	TimeMark mInternalTimeMark;

public:
	DE_CLASS(Time, DE_Class)

	void init() { mInternalTimeMark.init() ; }
	void startFrame() { mInternalTimeMark.startFrame() ; }
	void endFrame() { mInternalTimeMark.endFrame() ; }
	f32 getElapsedTime() { return mInternalTimeMark.getElapsedTime() ; }
	f32 getDeltaTimeMillis() { return mInternalTimeMark.getDeltaTimeMillis() ; }
	f32 getDeltaTimeSeconds() { return mInternalTimeMark.getDeltaTimeSeconds() ; }
};

} /* namespace DE */

#endif /* DE_TIME_H */
