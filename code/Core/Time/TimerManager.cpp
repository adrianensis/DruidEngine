#include "Core/Time/TimerManager.hpp"
#include "Log/Log.hpp"
#include "Profiler/Profiler.hpp"

void Timer::init(f32 duration, TimerDurationType durationType, std::function<void()> callback)
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
	PROFILER_TIMEMARK_START()
	if (!mTimers.empty())
	{
		f32 deltaTime = Time::getInstance().getDeltaTimeSeconds(); // seconds
		std::list<Timer *> timers(mTimers);

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

	PROFILER_TIMEMARK_END()
}

TimerHandle TimerManager::setTimer(f32 duration, TimerDurationType durationType, std::function<void()> callback)
{
	Timer *timer = NEW(Timer);
	timer->init(duration, durationType, callback);

	TimerHandle timerHandler;
	timerHandler.init(timer);

	mTimers.push_back(timer);

	return timerHandler;
}

void TimerManager::cancelTimer(const TimerHandle &timerHandle)
{
	endTimer(timerHandle.mTimerReference);
}

void TimerManager::terminate()
{
	mTimers.clear();
}