#include <TimerManager.hpp>
#include "Log.hpp"
#include "List.hpp"
#include "Memory.hpp"

namespace DE {


TimerHandle::TimerHandle() : DE_Class(){
	mTimerReference = nullptr;
}

TimerHandle::~TimerHandle() = default;

Timer::Timer() : DE_Class(), Singleton<Timer>() { };

Timer::~Timer() = default;

// ---------------------------------------------------------------------------

TimerManager::TimerManager() : DE_Class(), Singleton<TimerManager>(){
	mTimers = nullptr;
}

TimerManager::~TimerManager() {
	if(mTimers){
		Memory::free<List<Timer*>>(mTimers);
	}
}

void TimerManager::init(){
	mTimers = Memory::allocate<List<Timer*>>();
	mTimers->init();
}

void TimerManager::endTimer(Timer* timer) {
	mTimers->remove(timer);
	Memory::free<Timer>(timer);
}

void TimerManager::step(f32 deltaTime) {

	List<Timer*>* timers = Memory::allocate<List<Timer*>>();
	timers->init(*mTimers);

	FOR_LIST(itTimer, timers){
		Timer* timer = itTimer.get();

		timer->mTimeCounter += deltaTime;

		VAR(f32, timer->mTimeCounter);

		if(timer->mTimeCounter >= timer->mDuration){
			timer->mFunctor.execute();
			endTimer(timer);
		}
	}

	Memory::free<List<Timer*>>(timers);
}

TimerHandle TimerManager::setTimer(f32 duration, TimerDurationType durationType, std::function<void()> callback) {

	Timer* timer = Memory::allocate<Timer>();
	timer->mDurationType = durationType;
	timer->mDuration = duration;
	timer->mFunctor.setCallback(callback);

	TimerHandle timerHandler;
	timerHandler.init(timer);

	mTimers->pushBack(timer);

	return timerHandler;
}

void TimerManager::cancelTimer(const TimerHandle &timerHandle) {
	endTimer(timerHandle.mTimerReference);
}

void TimerManager::terminate(){
	mTimers->clear();
}

} /* namespace DE */
