#pragma once

#include "Core/Core.hpp"
#include <chrono>

CLASS(TimeMark, ObjectBase)
{
	PRI_M(f32, DeltaTimeMillis, NONE)
	PRI_M(f32, DeltaTimeSeconds, NONE)
	PRI_M(std::chrono::milliseconds, DeltaTimeChronoDuration, NONE);
	PRI_M(std::chrono::time_point<std::chrono::high_resolution_clock>, StartTime, NONE);
	PRI_M(std::chrono::time_point<std::chrono::high_resolution_clock>, LastTime, NONE);
	PRI_M(bool, IsStarted, GET)

PUB
	void init();
	void start();
	void end();
	f32 getElapsedTimeMillis();
	f32 getDeltaTimeMillis();
	f32 getDeltaTimeSeconds();

	COPY(TimeMark)
	{
		DO_COPY(DeltaTimeMillis)
		DO_COPY(DeltaTimeSeconds)
		DO_COPY(DeltaTimeChronoDuration)
		DO_COPY(StartTime)
		DO_COPY(LastTime)
	}
};

CLASS(Time, ObjectBase), SINGLETON(Time)
{	
	PRI_M(TimeMark, InternalTimeMark, NONE)

PUB

	void init(){mInternalTimeMark.init();}
	void startFrame() { mInternalTimeMark.start(); }
	void endFrame() { mInternalTimeMark.end(); }
	f32 getElapsedTimeMillis() { return mInternalTimeMark.getElapsedTimeMillis(); }
	f32 getDeltaTimeMillis() { return mInternalTimeMark.getDeltaTimeMillis(); }
	f32 getDeltaTimeSeconds() { return mInternalTimeMark.getDeltaTimeSeconds(); }
};
