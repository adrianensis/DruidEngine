#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/Time/TimerManager.hpp"

#define EVENT_DECLARATION_BEGIN(EventClassName) CLASS(EventClassName, Event) { \
public: \
	EventClassName() {}; \
	virtual ~EventClassName() override {}; \

#define EVENT_DECLARATION_END(EventClassName) };

CLASS(Event, ObjectBase) {

	PUB(DelayAmount, NONE, f32)
	PUB(DelayType, NONE, TimerDurationType)
	PUB(Instigator, NONE, ObjectBase*)

public:
	// NOTE : Override in children!
	Event& operator= (const Event &event) {
	   // self-assignment guard
	   if (this == &event)
	       return *this;

	   // do the copy
	   mInstigator = event.mInstigator; // can handle self-assignment
	   mDelayType = event.mDelayType; // can handle self-assignment
	   mDelayAmount = event.mDelayAmount;

	   return *this;
	}
};

using EventCallback = std::function<void(const Event*)>;

template<class E>
class EventFunctor : public Functor<EventCallback> {
public:

	PUB(Event, NONE, E*)
	PUB(EventClassId, NONE, ClassId)
	PUB(EventReceiver, NONE, ObjectBase*)

	EventFunctor():Functor<EventCallback>() {
		mEvent = nullptr;
		mEventClassId = 0;
		mEventReceiver = nullptr;
	};
	~EventFunctor() override {};

	void execute() override {
		if(mCallback){
			mCallback(mEvent);
		}
	}

	// NOTE : Override in children!
	EventFunctor& operator= (const EventFunctor &eventFunctor) {
		// self-assignment guard
		if (this == &eventFunctor)
			return *this;

		// do the copy
		Functor::operator=(eventFunctor);
		mEvent = eventFunctor.mEvent;
		mEventClassId = eventFunctor.mEventClassId;
		mEventReceiver = eventFunctor.mEventReceiver;

		return *this;
	}

	bool operator== (const EventFunctor& eventFunctor) const{
		return mEventClassId == eventFunctor.mEventClassId && mEventReceiver == eventFunctor.mEventReceiver;
	}
};