#pragma once

#include "Core/Time/TimeUtils.hpp"
#include "Core/Core.hpp"
#include "Core/Functor.hpp"

enum class TimerDurationType
{
	NONE,
	TIME_AMOUNT,
	NEXT_FRAME
};

CLASS(Timer, ObjectBase)
{
	PRI(f32, Duration, GET)
	PRI(f32, TimeCounter, GET_SET)
	PRI(TimerDurationType, DurationType, GET)
	PUB(FunctorVoid, Functor, NONE)

public:
	void init(f32 duration, TimerDurationType durationType, SFun(void()) callback);
};

CLASS(TimerHandle, ObjectBase)
{
	friend class TimerManager;

	PRI(Timer *, TimerReference, NONE)

public:
	void init(Timer * timerReference)
	{
		mTimerReference = timerReference;
	}

	COPY(TimerHandle)
	{
		DO_COPY(TimerReference)
	}
};

CLASS(TimerManager, ObjectBase), SINGLETON(TimerManager)
{
	PRI(SLst(Timer *), Timers, NONE);

	void endTimer(Timer * timer);

public:
	~TimerManager() OVR;
	void init();
	TimerHandle setTimer(f32 duration, TimerDurationType durationType, SFun(void()) callback);
	void cancelTimer(const TimerHandle &timerHandle);
	void update();
	void terminate();
};