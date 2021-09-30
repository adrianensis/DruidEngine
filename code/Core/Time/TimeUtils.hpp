#pragma once

#include "Core/Core.hpp"
#include <chrono>

CLASS(TimeMark, ObjectBase)
{
	PRI(f32, DeltaTimeMillis, NONE)
	PRI(f32, DeltaTimeSeconds, NONE)
	PRI(std::chrono::milliseconds, DeltaTimeChronoDuration, NONE);
	PRI(std::chrono::time_point<std::chrono::high_resolution_clock>, StartTime, NONE);
	PRI(std::chrono::time_point<std::chrono::high_resolution_clock>, LastTime, NONE);
	PRI(bool, IsStarted, GET)

public:
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
	PRI(TimeMark, InternalTimeMark, NONE)

public :

	void init(){mInternalTimeMark.init();}
	void startFrame() { mInternalTimeMark.start(); }
	void endFrame() { mInternalTimeMark.end(); }
	f32 getElapsedTimeMillis() { return mInternalTimeMark.getElapsedTimeMillis(); }
	f32 getDeltaTimeMillis() { return mInternalTimeMark.getDeltaTimeMillis(); }
	f32 getDeltaTimeSeconds() { return mInternalTimeMark.getDeltaTimeSeconds(); }
};
