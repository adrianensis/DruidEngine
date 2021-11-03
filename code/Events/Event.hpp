#pragma once

#include "Core/Core.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/Time/TimerManager.hpp"

class Event: public ObjectBase
{
    GENERATE_METADATA(Event)
	PUB_M(f32, DelayAmount, NONE)
	PUB_M(TimerDurationType, DelayType, NONE)
	PUB_M(ObjectBase *, Instigator, NONE)

	PUB
		// NOTE : Override in children!
		COPY(Event)
		{
			DO_COPY(Instigator)
			DO_COPY(DelayType)
			DO_COPY(DelayAmount)
		}
};

using EventCallback = SFun(void(const Event *));

template<class E>
class EventFunctor: public Functor<EventCallback>
{
	GENERATE_METADATA(EventFunctor<E>)
	PUB_M(E *, Event, NONE)
	PUB_M(ClassId, EventClassId, NONE)
	PUB_M(ObjectBase *, EventReceiver, NONE)

PUB
	void execute() override
	{
		if (mCallback)
		{
			mCallback(mEvent);
		}
	}

	// NOTE : Override in children!
	COPY(EventFunctor)
	{
		Functor<EventCallback>::copy(other);
		DO_COPY(Event)
		DO_COPY(EventClassId)
		DO_COPY(EventReceiver)
	}

	bool operator==(const EventFunctor &eventFunctor) const
	{
		return mEventClassId == eventFunctor.mEventClassId && mEventReceiver == eventFunctor.mEventReceiver;
	}
};