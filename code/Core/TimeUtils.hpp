#pragma once

#include "Core/Core.hpp"
#include <chrono>

CLASS(TimeMark, ObjectBase) {

	PRI(DeltaTimeMillis, NONE, f32)
	PRI(DeltaTimeSeconds, NONE, f32)
	PRI(DeltaTimeChronoDuration, NONE, std::chrono::milliseconds);
	PRI(StartTime, NONE, std::chrono::time_point<std::chrono::high_resolution_clock>);
	PRI(LastTime, NONE, std::chrono::time_point<std::chrono::high_resolution_clock>);
	PRI(IsStarted, GET, bool)

public:

	void init();
	void start();
	void end();
	f32 getElapsedTimeMillis();
	f32 getDeltaTimeMillis();
	f32 getDeltaTimeSeconds();

	TimeMark& operator=(const TimeMark &rhs) {
		if (this == &rhs)
			return *this; // handle self assignment
		
		mDeltaTimeMillis = rhs.mDeltaTimeMillis;
		mDeltaTimeSeconds = rhs.mDeltaTimeSeconds;
		mDeltaTimeChronoDuration = rhs.mDeltaTimeChronoDuration;
		mStartTime = rhs.mStartTime;
		mLastTime = rhs.mLastTime;

		return *this;
	}
};

CLASS(Time, ObjectBase), SINGLETON(Time) {
private:
	PRI(InternalTimeMark, NONE, TimeMark)

public:

	void init() { mInternalTimeMark.init(); }
	void startFrame() { mInternalTimeMark.start(); }
	void endFrame() { mInternalTimeMark.end(); }
	f32 getElapsedTimeMillis() { return mInternalTimeMark.getElapsedTimeMillis(); }
	f32 getDeltaTimeMillis() { return mInternalTimeMark.getDeltaTimeMillis(); }
	f32 getDeltaTimeSeconds() { return mInternalTimeMark.getDeltaTimeSeconds(); }
};
