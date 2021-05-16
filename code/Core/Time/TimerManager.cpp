#include "Core/Time/TimerManager.hpp"
#include "Log/Log.hpp"

void Timer::init(f32 duration, TimerDurationType durationType, std::function<void()> callback){
	mDurationType = durationType;
	mDuration = duration;
	mFunctor.setCallback(callback);
}

TimerManager::~TimerManager(){
	TRACE()
	if(mTimers){
		delete mTimers;
	}
}

void TimerManager::init(){
	mTimers = new std::list<Timer*>;
}

void TimerManager::endTimer(Timer* timer){
	mTimers->remove(timer);
	delete timer;
}

void TimerManager::step(f32 deltaTime){

	if(! mTimers->empty()){
		std::list<Timer*> timers(*mTimers);

		for(auto itTimer = timers.begin(); itTimer != timers.end(); ++itTimer){
			Timer* timer = *itTimer;

			timer->setTimeCounter(timer->getTimeCounter() + deltaTime);

			//VAR(f32, timer->mTimeCounter);

			if(timer->getTimeCounter() >= timer->getDuration()){
				timer->mFunctor.execute();
				endTimer(timer);
			}
		}
	}
}

TimerHandle TimerManager::setTimer(f32 duration, TimerDurationType durationType, std::function<void()> callback){

	Timer* timer = new Timer;
	timer->init(duration, durationType, callback);

	TimerHandle timerHandler;
	timerHandler.init(timer);

	mTimers->push_back(timer);

	return timerHandler;
}

void TimerManager::cancelTimer(const TimerHandle &timerHandle){
	endTimer(timerHandle.mTimerReference);
}

void TimerManager::terminate(){
	if(mTimers){
		mTimers->clear();
	}
}