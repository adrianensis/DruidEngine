#include "Core/Profiler/Profiler.hpp"
#include "Core/Log/Log.hpp"

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
	mFrameCounter++;

	mTotalTimeMillis += deltaTimeMillis;
}

void Profiler::terminate()
{
	ECHO("")
	ECHO("PROFILER RESULTS")
	printResult("Total Time", mTotalTimeMillis);
	printResultAverage("Frame Time", mTotalTimeMillis);
	FOR_MAP(it, mTimeMap)
	{
		//printResult(it->first, it->second);
		printResultAverage(it->first, it->second);
		ECHO("----------------------------")
	}

	ECHO("")

	mTimeMap.clear();

	mTimeMarkMap.clear();
}

void Profiler::printResult(const std::string &name, f32 time)  const
{
	ECHO(name + " : " + std::to_string(time) + "ms ")
}

void Profiler::printResultAverage(const std::string &name, f32 time)  const
{
	printResult(std::string("(Avg) ") + name, time / mFrameCounter);
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