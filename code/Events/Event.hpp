#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/Time/TimerManager.hpp"

#define CLASS_EVENT(EventClassName) CLASS(EventClassName, Event)

CLASS(Event, ObjectBase){
	PUB(DelayAmount, NONE, f32)
	PUB(DelayType, NONE, TimerDurationType)
	PUB(Instigator, NONE, ObjectBase *)

	public :
		// NOTE : Override in children!
		COPY(Event)
		{
			DO_COPY(Instigator)
			DO_COPY(DelayType)
			DO_COPY(DelayAmount)
		}
};

using EventCallback = std::function<void(const Event *)>;

CLASS_TEMPLATE(EventFunctor, E, Functor<EventCallback>)
{
	PUB(Event, NONE, E *)
	PUB(EventClassId, NONE, ClassId)
	PUB(EventReceiver, NONE, ObjectBase *)

public:
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