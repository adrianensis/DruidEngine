#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/Singleton.hpp"
#include "Core/DE_Class.hpp"
#include "Log/Log.hpp"
#include <chrono>

namespace DE {

class TimeMark : public DE_Class {
private:
	DE_M(DeltaTimeMillis, f32)
	DE_M(DeltaTimeSeconds, f32)
	DE_M(DeltaTimeChronoDuration, std::chrono::milliseconds)
	DE_M(StartTime, std::chrono::time_point<std::chrono::high_resolution_clock>)
	DE_M(LastTime, std::chrono::time_point<std::chrono::high_resolution_clock>)

public:
	DE_CLASS(TimeMark)

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

class Time : public DE_Class, public Singleton<Time>{
private:
	DE_M(InternalTimeMark, TimeMark)

public:
	DE_CLASS(Time)

	void init() { mInternalTimeMark.init() ; }
	void startFrame() { mInternalTimeMark.startFrame() ; }
	void endFrame() { mInternalTimeMark.endFrame() ; }
	f32 getElapsedTime() { return mInternalTimeMark.getElapsedTime() ; }
	f32 getDeltaTimeMillis() { return mInternalTimeMark.getDeltaTimeMillis() ; }
	f32 getDeltaTimeSeconds() { return mInternalTimeMark.getDeltaTimeSeconds() ; }
};

}

