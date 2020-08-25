#include "EventsManager.hpp"
#include "HashMap.hpp"
#include "Memory.hpp"

namespace DE {

EventsManager::EventsManager() : DE_Class() {
	mEventsMap = nullptr;
}

EventsManager::~EventsManager() {
	if(mEventsMap){
		FOR_LIST(it, mEventsMap->getValues()) {
			Memory::free<HashMap<DE_Class*, EventFunctor<Event>>>(it.get());
		}

		Memory::free<HashMap<ClassId, HashMap<DE_Class*, EventFunctor<Event>>*>>(mEventsMap);
	}
}

void EventsManager::init(){
	mEventsMap = Memory::allocate<HashMap<ClassId, HashMap<DE_Class*, EventFunctor<Event>>*>>();
	mEventsMap->init();
}

void EventsManager::subscribe(ClassId eventClassId, DE_Class* eventReceiver, EventCallback eventCallback){

	HashMap<DE_Class*, EventFunctor<Event>>* subMap = nullptr;

	if(!mEventsMap->contains(eventClassId)){
		subMap = Memory::allocate<HashMap<DE_Class*, EventFunctor<Event>>>();
		subMap->init();

		mEventsMap->set(eventClassId, subMap);
	}

	EventFunctor<Event> eventFunctor;
	eventFunctor.setCallback(eventCallback);

	mEventsMap->get(eventClassId)->set(eventReceiver, eventFunctor);
}

void EventsManager::unsubscribe(ClassId eventClassId, DE_Class* eventReceiver){
	if(mEventsMap->contains(eventClassId)){
		if(mEventsMap->get(eventClassId)->contains(eventReceiver)){
			mEventsMap->get(eventClassId)->remove(eventReceiver);
		}
	}
}

void EventsManager::send(DE_Class* sender, Event* event) {
	ClassId eventClassId = event->getClassId();
	if(mEventsMap->contains(eventClassId)){
		FOR_LIST(it, mEventsMap->get(eventClassId)->getValues()){
			EventFunctor<Event> functor = it.get();
			functor.mEvent = event;
			functor.mEvent->mSender = sender;
			functor.execute();
		}
	}
}

void EventsManager::terminate(){
	if(mEventsMap){
		FOR_LIST(it, mEventsMap->getValues()) {
			Memory::free<HashMap<DE_Class*, EventFunctor<Event>>>(it.get());
		}

		mEventsMap->clear();
	}
}

} /* namespace DE */
