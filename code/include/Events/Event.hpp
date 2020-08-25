#ifndef DE_EVENT_H_
#define DE_EVENT_H_

#include "DE_Class.hpp"
#include "Functor.hpp"

namespace DE {

/*
  Macros to declare new events. Example:

  DE_EVENT_DECLARATION_BEGIN(EventDamage);

  // Attributes go here
  f32 damage;
  f32 damageMultiplier;

  DE_EVENT_DECLARATION_END();
 */

#define DE_EVENT_DECLARATION_BEGIN(EventClassName) class EventClassName : public Event{ \
public: \
	DE_GENERATE_METADATA(EventClassName, Event); \
	EventClassName() = default; \
~EventClassName() override = default; \

#define DE_EVENT_DECLARATION_END(EventClassName) };

enum class EventDelay {
	NONE,
	TIMER,
	NEXT_FRAME
};

class Event : public DE_Class{

public:
	DE_CLASS(Event, DE_Class);

	DE_Class* mSender;

	EventDelay mDelayType;
	f32 mDelayAmount;

	// NOTE : Override in children!
	Event& operator= (const Event &event) {
	    // self-assignment guard
	    if (this == &event)
	        return *this;

	    // do the copy
	    mSender = event.mSender; // can handle self-assignment
	    mDelayType = event.mDelayType; // can handle self-assignment
	    mDelayAmount = event.mDelayAmount;

	    return *this;
	}
};

using EventCallback = std::function<void(const Event*)>;

template<class E>
class EventFunctor : public Functor<EventCallback> {

public:
	E* mEvent = nullptr;

	EventFunctor() = default;
	~EventFunctor() override = default;

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

		return *this;
	}

	bool operator==(const EventFunctor &rhs) const {
		return this == &rhs;
	}
};

} /* namespace DE */

#endif /* DE_EVENT_H_ */
