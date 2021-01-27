#include "Core/TimerManager.hpp"
#include "Log/Log.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

TimerHandle::TimerHandle() : DE_Class(){
	mTimerReference = nullptr;
}

TimerHandle::~TimerHandle() = default;

Timer::Timer() : DE_Class() {
	mDuration = 0;
	mTimeCounter = 0;
	mDurationType = TimerDurationType::TIME_AMOUNT;
};

Timer::~Timer() = default;

void Timer::init(f32 duration, TimerDurationType durationType, std::function<void()> callback) {
	mDurationType = durationType;
	mDuration = duration;
	mFunctor.setCallback(callback);
}

TimerManager::TimerManager() : DE_Class(), Singleton<TimerManager>(){
	mTimers = nullptr;
}

TimerManager::~TimerManager() {
	if(mTimers){
		DE_FREE(mTimers);
	}
}

void TimerManager::init(){
	mTimers = DE_NEW<List<Timer*>>();
	mTimers->init();
}

void TimerManager::endTimer(Timer* timer) {
	mTimers->remove(timer);
	DE_FREE(timer);
}

void TimerManager::step(f32 deltaTime) {

	if(! mTimers->isEmpty()){
		List<Timer*>* timers = DE_NEW<List<Timer*>>();
		timers->init(*mTimers);

		FOR_LIST(itTimer, timers){
			Timer* timer = itTimer.get();

			timer->setTimeCounter(timer->getTimeCounter() + deltaTime);

			//VAR(f32, timer->mTimeCounter);

			if(timer->getTimeCounter() >= timer->getDuration()){
				timer->mFunctor.execute();
				endTimer(timer);
			}
		}

		DE_FREE(timers);
	}
}

TimerHandle TimerManager::setTimer(f32 duration, TimerDurationType durationType, std::function<void()> callback) {

	Timer* timer = DE_NEW<Timer>();
	timer->init(duration, durationType, callback);

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

}
