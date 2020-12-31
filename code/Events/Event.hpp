#ifndef DE_EVENT_H_
#define DE_EVENT_H_

#include "DE_Class.hpp"
#include "BasicTypes.hpp"
#include "Functor.hpp"
#include "TimerManager.hpp"

namespace DE {

/*
  Macros to declare new events. Example:

  DE_EVENT_DECLARATION_BEGIN(EventDamage);

  // MEMBERs go here
  f32 damage;
  f32 damageMultiplier;

  DE_EVENT_DECLARATION_END();
 */

#define DE_EVENT_DECLARATION_BEGIN(EventClassName) class EventClassName : public Event{ \
public: \
	DE_GENERATE_METADATA(EventClassName, Event); \
	EventClassName():Event() {}; \
	~EventClassName() override {}; \

#define DE_EVENT_DECLARATION_END(EventClassName) };

class Event : public DE_Class{
public:
	DE_CLASS(Event, DE_Class)
	
	DE_M(DelayAmount, f32)
	DE_M(DelayType, TimerDurationType)
	DE_M(Instigator, DE_Class*);

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
	DE_GENERATE_METADATA(EventFunctor, Functor<EventCallback>);

	DE_M(Event, E*)
	DE_M(EventClassId, ClassId)
	DE_M(EventReceiver, DE_Class*);

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

} /* namespace DE */

#endif /* DE_EVENT_H_ */
