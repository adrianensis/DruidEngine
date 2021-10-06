#pragma once

#include "Core/Core.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/Time/TimerManager.hpp"

#define CLASS_EVENT(EventClassName) CLASS(EventClassName, Event)

CLASS(Event, ObjectBase){
	PUB(f32, DelayAmount, NONE)
	PUB(TimerDurationType, DelayType, NONE)
	PUB(ObjectBase *, Instigator, NONE)

	public :
		// NOTE : Override in children!
		COPY(Event)
		{
			DO_COPY(Instigator)
			DO_COPY(DelayType)
			DO_COPY(DelayAmount)
		}
};

using EventCallback = SFun(void(const Event *));

CLASS_TEMPLATE(EventFunctor, E, Functor<EventCallback>)
{
	PUB(E *, Event, NONE)
	PUB(ClassId, EventClassId, NONE)
	PUB(ObjectBase *, EventReceiver, NONE)

public:
	void execute() OVR
	{
		if (mCallback)
		{
			mCallback(mEvent);
		}
	}

	// NOTE : Override in children!
	COPY(EventFunctor)
	{
		Super::copy(other);
		DO_COPY(Event)
		DO_COPY(EventClassId)
		DO_COPY(EventReceiver)
	}

	bool operator==(const EventFunctor &eventFunctor) const
	{
		return mEventClassId == eventFunctor.mEventClassId && mEventReceiver == eventFunctor.mEventReceiver;
	}
};