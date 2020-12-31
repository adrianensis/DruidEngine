#ifndef DE_TIMERMANAGER_H
#define DE_TIMERMANAGER_H

#include "Core/TimeUtils.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Singleton.hpp"
#include "Core/DE_Class.hpp"
#include "Core/Functor.hpp"

namespace DE {

template<class T> class List;

enum class TimerDurationType {
	NONE,
	TIME_AMOUNT,
	NEXT_FRAME
};

class TimerManager;

class Timer : public DE_Class{
private:
	DE_M_GET(Duration, f32)
	DE_M_GET_SET(TimeCounter, f32)
	DE_M_GET(DurationType ,TimerDurationType)

public:
	DE_CLASS(Timer, DE_Class)

	DE_M(Functor, FunctorVoid);

	void init(f32 duration, TimerDurationType durationType, std::function<void()> callback);
};

class TimerHandle : public DE_Class{

	friend TimerManager;

private:
	DE_M(TimerReference, Timer*);

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

class TimerManager : public DE_Class, public Singleton<TimerManager>{
private:
	DE_M(Timers, List<Timer*>*);

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
