#pragma once

#include "Core/Core.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/Time/TimerManager.hpp"

class Event: public ObjectBase
{
    GENERATE_METADATA(Event)
	PUB f32 mDelayAmount = 0.0f;
	PUB TimerDurationType mDelayType;
	PUB ObjectBase* mInstigator = nullptr;

	PUB
		// NOTE : Override in children!
		COPY(Event)
		{
			DO_COPY(mInstigator)
			DO_COPY(mDelayType)
			DO_COPY(mDelayAmount)
		}
};

using EventCallback = std::function<void(const Event *)>;

template<class E>
class EventFunctor: public Functor<EventCallback>
{
	GENERATE_METADATA(EventFunctor<E>)
	PUB E* mEvent = nullptr;
	PUB ClassId mEventClassId;
	PUB ObjectBase* mEventReceiver = nullptr;

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
		DO_COPY(mEvent)
		DO_COPY(mEventClassId)
		DO_COPY(mEventReceiver)
	}

	bool operator==(const EventFunctor &eventFunctor) const
	{
		return mEventClassId == eventFunctor.mEventClassId && mEventReceiver == eventFunctor.mEventReceiver;
	}
};