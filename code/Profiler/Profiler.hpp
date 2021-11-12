#pragma once

#include "Core/Time/TimerManager.hpp"
#include "Core/Core.hpp"

#define PROFILER_TIMEMARK_START() Profiler::getInstance().timeMarkStart(__PRETTY_FUNCTION__);
#define PROFILER_TIMEMARK_END() Profiler::getInstance().timeMarkEnd(__PRETTY_FUNCTION__);

class Profiler : public ObjectBase, public Singleton<Profiler>
{
	GENERATE_METADATA(Profiler)

	PRI std::map<std::string, f32> mTimeMap;
	PRI std::map<std::string, TimeMark> mTimeMarkMap;

	PRI f32 mTotalTimeMillis = 0;
	PRI u32 mFrameCounter = 0;

	PRI void printResult(const std::string& name, f32 time) const;
	PRI void printResultAverage(const std::string& name, f32 time) const;

PUB
	Profiler();
	~Profiler();

	void init();
	void update(f32 deltaTimeMillis);
	void terminate();

	void timeMarkStart(const std::string& name);
	void timeMarkEnd(const std::string& name);
};
