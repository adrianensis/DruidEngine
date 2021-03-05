#pragma once

#include "Core/DE_Class.hpp"
#include "Events/Event.hpp"
#include "Assert/Assert.hpp"
#include "Core/Singleton.hpp"

namespace DE {

/*
  Macros for (un)susbscribing and sending events.
*/

#define DE_SUBSCRIBE_TO_EVENT(EventClassName, owner, receiver, eventCallback) EventsManager::getInstance()->subscribe<EventClassName>(owner, receiver, eventCallback);
#define DE_UNSUBSCRIBE_TO_EVENT(EventClassName, owner, receiver) EventsManager::getInstance()->unsubscribe<EventClassName>(owner, receiver);
#define DE_SEND_EVENT(owner, instigator, event) EventsManager::getInstance()->send(owner, instigator, &event);

template<class K, class V> class HashMap;
template<class T> class List;

class EventsManager : public DE_Class, public Singleton<EventsManager>{

private:

	using ReceiverFunctorsMap = HashMap<DE_Class*, EventFunctor<Event>>;
	using EventReceiversMap = HashMap<ClassId, ReceiverFunctorsMap*>;
	using OwnersReceiversMap = HashMap<DE_Class*, EventReceiversMap*>;

	OwnersReceiversMap* mOwnersReceiversMap;

	void removeMapContent();

	void subscribe(ClassId eventClassId, DE_Class* eventOwner, DE_Class* eventReceiver, EventCallback eventCallback);
	void unsubscribe(ClassId eventClassId, DE_Class* eventOwner, DE_Class* eventReceiver);

public:
	DE_CLASS_BODY(EventsManager)

	void init();
	void terminate();

	template<class E>
	void subscribe(DE_Class* eventOwner, DE_Class* eventReceiver, EventCallback eventCallback){
		if(std::is_base_of<Event, E>::value){
			subscribe(E::getClassIdStatic(), eventOwner, eventReceiver, eventCallback);
		} else {
			DE_ASSERT(false, "Class must be derived from Event.");
		}
	}

	template<class E>
	void unsubscribe(DE_Class* eventOwner, DE_Class* eventReceiver){
		if(std::is_base_of<Event, E>::value){
			unsubscribe(E::getClassIdStatic(), eventOwner, eventReceiver);
		} else {
			DE_ASSERT(false, "Class must be derived from Event.");
		}
	}

	void send(DE_Class* eventOwner, DE_Class* eventInstigator, Event* event);
};

}

