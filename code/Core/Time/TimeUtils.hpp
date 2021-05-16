#pragma once

#include "Core/Core.hpp"
#include <chrono>

CLASS(TimeMark, ObjectBase){

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

	COPY(TimeMark){
		DO_COPY(DeltaTimeMillis)
		DO_COPY(DeltaTimeSeconds)
		DO_COPY(DeltaTimeChronoDuration)
		DO_COPY(StartTime)
		DO_COPY(LastTime)		
	}
};

CLASS(Time, ObjectBase), SINGLETON(Time){
	PRI(InternalTimeMark, NONE, TimeMark)

public:

	void init(){ mInternalTimeMark.init(); }
	void startFrame(){ mInternalTimeMark.start(); }
	void endFrame(){ mInternalTimeMark.end(); }
	f32 getElapsedTimeMillis(){ return mInternalTimeMark.getElapsedTimeMillis(); }
	f32 getDeltaTimeMillis(){ return mInternalTimeMark.getDeltaTimeMillis(); }
	f32 getDeltaTimeSeconds(){ return mInternalTimeMark.getDeltaTimeSeconds(); }
};
