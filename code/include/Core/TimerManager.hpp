#ifndef DE_TIMERMANAGER_H
#define DE_TIMERMANAGER_H

#include "BasicTypes.hpp"
#include "Singleton.hpp"
#include "DE_Class.hpp"
#include "Functor.hpp"
#include "Time.hpp"

namespace DE {

template<class T> class List;

enum class TimerDurationType {
	NONE,
	TIME_AMOUNT,
	NEXT_FRAME
};

class Timer;
class TimerManager;

class TimerHandle : public DE_Class{

	friend TimerManager;

private:
	Timer* mTimerReference;

public:
	DE_CLASS(TimerHandle, DE_Class)

	void init(Timer* timerReference){
		mTimerReference = timerReference;
	}

	TimerHandle& operator= (const TimerHandle &timerHandle) {
		// self-assignment guard
		if (this == &timerHandle)
			return *this;

		// do the copy
		mTimerReference = timerHandle.mTimerReference;

		return *this;
	}
};

class Timer : public DE_Class, public Singleton<Timer>{

public:
	DE_CLASS(Timer, DE_Class)

	f32 mDuration = 0;
	f32 mTimeCounter = 0;
	TimerDurationType mDurationType = TimerDurationType::TIME_AMOUNT;
	FunctorVoid mFunctor;
};

class TimerManager : public DE_Class, public Singleton<TimerManager>{
private:
	List<Timer*>* mTimers;

	void endTimer(Timer* timer);

public:
	DE_CLASS(TimerManager, DE_Class)

	void init();
	TimerHandle setTimer(f32 duration, TimerDurationType durationType, std::function<void()> callback);
	void cancelTimer(const TimerHandle& timerHandle);

	void step(f32 deltaTime /*Seconds*/);

	void terminate();

};

} /* namespace DE */

#endif /* DE_TIMERMANAGER_H */
