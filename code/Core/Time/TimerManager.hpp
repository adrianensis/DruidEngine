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

class Timer: public ObjectBase
{
    GENERATE_METADATA(Timer)
	PRI_M(f32, Duration, GET)
	PRI_M(f32, TimeCounter, GET_SET)
	PRI_M(TimerDurationType, DurationType, GET)
	PUB_M(FunctorVoid, Functor, NONE)

PUB
	void init(f32 duration, TimerDurationType durationType, SFun(void()) callback);
};

class TimerHandle: public ObjectBase
{
    GENERATE_METADATA(TimerHandle)
	friend class TimerManager;

	PRI_M(Timer *, TimerReference, NONE)

PUB
	void init(Timer * timerReference)
	{
		mTimerReference = timerReference;
	}

	COPY(TimerHandle)
	{
		DO_COPY(TimerReference)
	}
};

class TimerManager: public ObjectBase, public Singleton<TimerManager>
{
	GENERATE_METADATA(TimerManager)
	PRI_M(SLst(Timer *), Timers, NONE);

	void endTimer(Timer * timer);

PUB
	~TimerManager() override;
	void init();
	TimerHandle setTimer(f32 duration, TimerDurationType durationType, SFun(void()) callback);
	void cancelTimer(const TimerHandle &timerHandle);
	void update();
	void terminate();
};