#include "Profiler/Profiler.hpp"

#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

Profiler::Profiler() : DE_Class(), Singleton<Profiler>() {

}

Profiler::~Profiler() {
	DE_FREE(mTimeMap);

	FOR_LIST(it, mTimeMarkMap->getValues()){
		DE_FREE(it.get());
	}

	DE_FREE(mTimeMarkMap);
}

void Profiler::init(){
	mTimeMap = DE_NEW<TimeMap>();
	mTimeMap->init();

	mTimeMarkMap = DE_NEW<TimeMarkMap>();
	mTimeMarkMap->init();
}

void Profiler::step(f32 deltaTime){
	mTotalTime += deltaTime;
}

void Profiler::terminate(){

	ECHO("")
	ECHO("PROFILER RESULTS")
	String totalTimeStr = "Total Time";
	printResult(totalTimeStr, mTotalTime);
	FOR_LIST(it, mTimeMap->getKeys()){
		printResult(it.get(), mTimeMap->get(it.get()));
	}

	ECHO("")

	mTimeMap->clear();

	FOR_LIST(it, mTimeMarkMap->getValues()){
		DE_FREE(it.get());
	}

	mTimeMarkMap->clear();
}

void Profiler::printResult(String &name, f32 time) {
	f32 percent = (time/mTotalTime) * 100.0f;
	ECHO(name + " : " + std::to_string(time) + "s : " + std::to_string(percent) + "%")
} 

void Profiler::timeMarkStart(const String &name) {

	// TODO: make it frame-time relative, maybe average?
	if(!mTimeMap->contains(name)){
		mTimeMap->set(name, 0);

		TimeMark* timeMark = DE_NEW<TimeMark>();
		mTimeMarkMap->set(name, timeMark);
	}

	mTimeMarkMap->get(name)->init();
	mTimeMarkMap->get(name)->startFrame();
}

void Profiler::timeMarkEnd(const String &name) {

	if(mTimeMap->contains(name)){
		mTimeMarkMap->get(name)->endFrame();
		mTimeMap->set(name, mTimeMap->get(name) + mTimeMarkMap->get(name)->getDeltaTimeSeconds());
	}
}

}
