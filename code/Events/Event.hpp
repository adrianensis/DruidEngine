#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/BasicTypes.hpp"
#include "Core/Functor.hpp"
#include "Core/TimerManager.hpp"

namespace DE {

#define EVENT_DECLARATION_BEGIN(EventClassName) class EventClassName : public Event{ \
public: \
	GENERATE_METADATA(EventClassName); \
	EventClassName():Event() {}; \
	~EventClassName() override {}; \

#define EVENT_DECLARATION_END(EventClassName) };

class Event : public ObjectBase{
public:
	GENERATE_METADATA(Event);

	Event();
	virtual ~Event() override;

	 f32 mDelayAmount;
	 TimerDurationType mDelayType;
	 ObjectBase* mInstigator;

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
	GENERATE_METADATA(EventFunctor);

	 E* mEvent;
	 ClassId mEventClassId;
	 ObjectBase* mEventReceiver;

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
}

