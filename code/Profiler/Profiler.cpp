#include "Profiler/Profiler.hpp"

#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

Profiler::Profiler() : DE_Class(), Singleton<Profiler>() {

}

Profiler::~Profiler() {
	Memory::free<TimeMap>(mTimeMap);

	FOR_LIST(it, mTimeMarkMap->getValues()){
		Memory::free<TimeMark>(it.get());
	}

	Memory::free<TimeMarkMap>(mTimeMarkMap);
}

void Profiler::init(){
	mTimeMap = Memory::allocate<TimeMap>();
	mTimeMap->init();

	mTimeMarkMap = Memory::allocate<TimeMarkMap>();
	mTimeMarkMap->init();
}

void Profiler::step(f32 deltaTime){
	mTotalTime += deltaTime;
}

void Profiler::terminate(){

	ECHO("")
	ECHO("PROFILER RESULTS")
	std::string totalTimeStr = "Total Time";
	printResult(totalTimeStr, mTotalTime);
	FOR_LIST(it, mTimeMap->getKeys()){
		printResult(it.get(), mTimeMap->get(it.get()));
	}

	ECHO("")

	mTimeMap->clear();

	FOR_LIST(it, mTimeMarkMap->getValues()){
		Memory::free<TimeMark>(it.get());
	}

	mTimeMarkMap->clear();
}

void Profiler::printResult(std::string &name, f32 time) {
	f32 percent = (time/mTotalTime) * 100.0f;
	ECHO(name + " : " + std::to_string(time) + "s : " + std::to_string(percent) + "%")
} 

void Profiler::timeMarkStart(const std::string &name) {

	// TODO: make it frame-time relative, maybe average?
	if(!mTimeMap->contains(name)){
		mTimeMap->set(name, 0);

		TimeMark* timeMark = Memory::allocate<TimeMark>();
		mTimeMarkMap->set(name, timeMark);
	}

	mTimeMarkMap->get(name)->init();
	mTimeMarkMap->get(name)->startFrame();
}

void Profiler::timeMarkEnd(const std::string &name) {

	if(mTimeMap->contains(name)){
		mTimeMarkMap->get(name)->endFrame();
		mTimeMap->set(name, mTimeMap->get(name) + mTimeMarkMap->get(name)->getDeltaTimeSeconds());
	}
}

} /* namespace DE */
