#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/Time/TimerManager.hpp"

#define EVENT_DECLARATION_BEGIN(EventClassName) \
	CLASS(EventClassName, Event)                \
	{                                           \
	public:                                     \
		EventClassName(){};                     \
		~EventClassName() override{};

#define EVENT_DECLARATION_END(EventClassName) \
	};

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

template <class E>
class EventFunctor : public Functor<EventCallback>
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
		Functor::operator=(*other);
		DO_COPY(Event)
		DO_COPY(EventClassId)
		DO_COPY(EventReceiver)
	}

	bool operator==(const EventFunctor &eventFunctor) const
	{
		return mEventClassId == eventFunctor.mEventClassId && mEventReceiver == eventFunctor.mEventReceiver;
	}
};