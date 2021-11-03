#pragma once

#include "Core/Core.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/Time/TimerManager.hpp"

class Event: public ObjectBase
{
    GENERATE_METADATA(Event)
	PUB f32 mDelayAmount = {};
	PUB TimerDurationType mDelayType = {};
	PUB ObjectBase * mInstigator = {};

	PUB
		// NOTE : Override in children!
		COPY(Event)
		{
			DO_COPY(Instigator)
			DO_COPY(DelayType)
			DO_COPY(DelayAmount)
		}
};

using EventCallback = std::function<void(const Event *)>;

template<class E>
class EventFunctor: public Functor<EventCallback>
{
	GENERATE_METADATA(EventFunctor<E>)
	PUB E * mEvent = {};
	PUB ClassId mEventClassId = {};
	PUB ObjectBase * mEventReceiver = {};

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