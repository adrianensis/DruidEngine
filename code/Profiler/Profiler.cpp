#include "Profiler/Profiler.hpp"

Profiler::Profiler()
{

}

Profiler::~Profiler()
{
	mTimeMap.clear();
	mTimeMarkMap.clear();
}

void Profiler::init()
{

}

void Profiler::update(f32 deltaTimeMillis)
{
	mTotalTimeMillis += deltaTimeMillis;
}

void Profiler::terminate()
{
	ECHO("")
	ECHO("PROFILER RESULTS")
	std::string totalTimeStr = "Total Time";
	printResult(totalTimeStr, mTotalTimeMillis);
	FOR_MAP(it, mTimeMap)
	{
		printResult(it->first, it->second);
	}

	ECHO("")

	mTimeMap.clear();

	mTimeMarkMap.clear();
}

void Profiler::printResult(const std::string &name, f32 time)  const
{
	f32 percent = (time/mTotalTimeMillis) * 100.0f;
	ECHO(name + " : " + std::to_string(time) + "ms, " + std::to_string(time / 1000.0f) + "s : " + std::to_string(percent) + "%")
} 

void Profiler::timeMarkStart(const std::string &name)
{
	// TODO: make it frame-time relative, maybe average?
	if(!MAP_CONTAINS(mTimeMap, name))
	{
		MAP_INSERT(mTimeMap, name, 0);

		TimeMark timeMark;
		MAP_INSERT(mTimeMarkMap, name, timeMark);
	}

	mTimeMarkMap.at(name).init();
	mTimeMarkMap.at(name).start();
}

void Profiler::timeMarkEnd(const std::string &name)
{
	if(MAP_CONTAINS(mTimeMap, name))
	{
		mTimeMarkMap.at(name).end();
		MAP_INSERT(mTimeMap, name, mTimeMap.at(name) + mTimeMarkMap.at(name).getDeltaTimeMillis());
	}
}