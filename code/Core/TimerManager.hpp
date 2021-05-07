#pragma once

#include "Core/TimeUtils.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Singleton.hpp"
#include "Core/ObjectBase.hpp"
#include "Core/Functor.hpp"
#include "Containers/List.hpp"

namespace DE {

enum class TimerDurationType {
	NONE,
	TIME_AMOUNT,
	NEXT_FRAME
};

class Timer : public ObjectBase{
private:
	PRIVATE(Duration, GET, f32)
	PRIVATE(TimeCounter, GET_SET, f32)
	PRIVATE(DurationType, GET, TimerDurationType)

public:

	GENERATE_METADATA(Timer);

	Timer();
	virtual ~Timer() override;

	PUBLIC(Functor, NONE, FunctorVoid)

	void init(f32 duration, TimerDurationType durationType, std::function<void()> callback);
};

class TimerHandle : public ObjectBase{

	friend class TimerManager;

private:
PRIVATE(TimerReference, NONE, Timer*)

public:
	GENERATE_METADATA(TimerHandle);

	TimerHandle();
	virtual ~TimerHandle() override;

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

class TimerManager : public ObjectBase, public Singleton<TimerManager>{
private:
	List<Timer*>* mTimers;

	void endTimer(Timer* timer);

public:
	GENERATE_METADATA(TimerManager);

	TimerManager();
	virtual ~TimerManager() override;

	void init();
	TimerHandle setTimer(f32 duration, TimerDurationType durationType, std::function<void()> callback);
	void cancelTimer(const TimerHandle& timerHandle);
	void step(f32 deltaTime /*Seconds*/);
	void terminate();

};
}

