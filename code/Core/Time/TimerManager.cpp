#include "Core/Time/TimerManager.hpp"
#include "Log/Log.hpp"

void Timer::init(f32 duration, TimerDurationType durationType, SFun(void()) callback)
{
	mDurationType = durationType;
	mDuration = duration;
	mFunctor.setCallback(callback);
}

TimerManager::~TimerManager()
{
	TRACE()
}

void TimerManager::init()
{

}

void TimerManager::endTimer(Timer *timer)
{
	mTimers.remove(timer);
	DELETE(timer);
}

void TimerManager::update()
{
	if (!mTimers.empty())
	{
		f32 deltaTime = Time::getInstance()->getDeltaTimeSeconds(); // seconds
		SLst(Timer *) timers(mTimers);

		for (auto itTimer = timers.begin(); itTimer != timers.end(); ++itTimer)
		{
			Timer *timer = *itTimer;

			timer->setTimeCounter(timer->getTimeCounter() + deltaTime);

			if (timer->getTimeCounter() >= timer->getDuration())
			{
				timer->mFunctor.execute();
				endTimer(timer);
			}
		}
	}
}

TimerHandle TimerManager::setTimer(f32 duration, TimerDurationType durationType, SFun(void()) callback)
{
	Timer *timer = NEW(Timer);
	timer->init(duration, durationType, callback);

	TimerHandle timerHandler;
	timerHandler.init(timer);

	mTimers.push_back(timer);

	return timerHandler;
}

void TimerManager::cancelTimer(CNS TimerHandle &timerHandle)
{
	endTimer(timerHandle.mTimerReference);
}

void TimerManager::terminate()
{
	mTimers.clear();
}