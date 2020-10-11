#include "EventsManager.hpp"
#include "HashMap.hpp"
#include "List.hpp"
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

	if(!mEventsMap->contains(eventClassId)){
		HashMap<DE_Class*, EventFunctor<Event>>* subMap = Memory::allocate<HashMap<DE_Class*, EventFunctor<Event>>>();
		subMap->init();

		mEventsMap->set(eventClassId, subMap);
	}

	EventFunctor<Event> eventFunctor;
	eventFunctor.setCallback(eventCallback);
	eventFunctor.mEventClassId = eventClassId;
	eventFunctor.mEventReceiver = eventReceiver;

	mEventsMap->get(eventClassId)->set(eventReceiver, eventFunctor);

	//TRACE();
	//VAR(u32, mEventsMap->get(eventClassId)->getValues()->getLength())
}

void EventsManager::unsubscribe(ClassId eventClassId, DE_Class* eventReceiver){
	if(mEventsMap->contains(eventClassId)){
		if(mEventsMap->get(eventClassId)->contains(eventReceiver)){
			mEventsMap->get(eventClassId)->remove(eventReceiver);
			//TRACE();
			//VAR(u32, mEventsMap->get(eventClassId)->getValues()->getLength())

			/*if(mEventsMap->get(eventClassId)->contains(eventReceiver)){
				ECHO("true")
			} else {
				ECHO("false")
			}*/
		}
	}
}

void EventsManager::send(DE_Class* sender, Event* event) {
	ClassId eventClassId = event->getClassId();
	if(mEventsMap->contains(eventClassId)){
		List<EventFunctor<Event>>* eventFunctors = Memory::allocate<List<EventFunctor<Event>>>();
		eventFunctors->init(*mEventsMap->get(eventClassId)->getValues());

		//TRACE();
		//VAR(u32, mEventsMap->get(eventClassId)->getValues()->getLength())
		FOR_LIST(it, mEventsMap->get(eventClassId)->getValues()){
			EventFunctor<Event> functor = it.get();
			functor.mEvent = event;
			functor.mEvent->mSender = sender;
			functor.execute();
		}
		//ECHO("End Send")
		//BRLINE()

		Memory::free<List<EventFunctor<Event>>>(eventFunctors);
	}
}

void EventsManager::step() {

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
