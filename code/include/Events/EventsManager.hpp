#ifndef DE_EVENTSMANAGER_H_
#define DE_EVENTSMANAGER_H_

#include "DE_Class.hpp"
#include "Event.hpp"
#include "Assert.hpp"
#include "Singleton.hpp"

namespace DE {

/*
  Macros for (un)susbscribing and sending events. Example:

  DE_EVENT_DECLARATION_BEGIN(EventDamage);

  // Attributes go here
  f32 damage;
  f32 damageMultiplier;

  DE_EVENT_DECLARATION_END();
 */

#define DE_SUBSCRIBE_TO_EVENT(EventClassName, eventCallback) EventsManager::getInstance()->subscribe<EventClassName>(this, eventCallback);
#define DE_UNSUBSCRIBE_TO_EVENT(EventClassName) EventsManager::getInstance()->unsubscribe<EventClassName>(this);
#define DE_SEND_EVENT_SPECIFIC_SENDER(sender, event) EventsManager::getInstance()->send(sender, &event);
#define DE_SEND_EVENT(event) DE_SEND_EVENT_SPECIFIC_SENDER(this, event);

template<class K, class V> class HashMap;
template<class T> class List;

enum class EventAction{
	SUBSCRIBE, UNSUBSCRIBE, SEND
};


class EventsManager : public DE_Class, public Singleton<EventsManager>{

private:

	HashMap<ClassId, HashMap<DE_Class*, EventFunctor<Event>>*>* mEventsMap;

	void subscribe(ClassId eventClassId, DE_Class* eventReceiver, EventCallback eventCallback);
	void unsubscribe(ClassId eventClassId, DE_Class* eventReceiver);

public:
	DE_CLASS(EventsManager, DE_Class);

	void init();
	void terminate();

	template<class E>
	void subscribe(DE_Class* eventReceiver, EventCallback eventCallback){
		if(std::is_base_of<Event, E>::value){
			subscribe(E::getClassIdStatic(), eventReceiver, eventCallback);
		} else {
			DE_ASSERT(false, "Class must be derived from Event.");
		}
	}

	template<class E>
	void unsubscribe(DE_Class* eventReceiver){
		if(std::is_base_of<Event, E>::value){
			unsubscribe(E::getClassIdStatic(), eventReceiver);
		} else {
			DE_ASSERT(false, "Class must be derived from Event.");
		}
	}

	void send(DE_Class* sender, Event* event);

	void step();
};


} /* namespace DE */

#endif /* DE_EVENTSMANAGER_H_ */
