#include "EventsManager.hpp"
#include "HashMap.hpp"
#include "List.hpp"
#include "Memory.hpp"

namespace DE {

EventsManager::EventsManager() : DE_Class(), Singleton<EventsManager>() {
	mOwnersReceiversMap = nullptr;
}

EventsManager::~EventsManager() {
	if(mOwnersReceiversMap){
		removeMapContent();
		Memory::free<OwnersReceiversMap>(mOwnersReceiversMap);
	}
}

void EventsManager::removeMapContent(){
	FOR_LIST(itEventReceivers, mOwnersReceiversMap->getValues()) {
		FOR_LIST(itFunctor, itEventReceivers.get()->getValues()) {
			Memory::free<ReceiverFunctorsMap>(itFunctor.get());
		}

		Memory::free<EventReceiversMap>(itEventReceivers.get());
	}
}

void EventsManager::init(){
	mOwnersReceiversMap = Memory::allocate<OwnersReceiversMap>();
	mOwnersReceiversMap->init();
}

void EventsManager::subscribe(ClassId eventClassId, DE_Class* eventOwner, DE_Class* eventReceiver, EventCallback eventCallback){

	if(!mOwnersReceiversMap->contains(eventOwner)){
		EventReceiversMap* eventsReceiverMap = Memory::allocate<EventReceiversMap>();
		eventsReceiverMap->init();
		mOwnersReceiversMap->set(eventOwner, eventsReceiverMap);
	}

	if(!mOwnersReceiversMap->get(eventOwner)->contains(eventClassId)){
		ReceiverFunctorsMap* receiversFunctorMap = Memory::allocate<ReceiverFunctorsMap>();
		receiversFunctorMap->init();
		mOwnersReceiversMap->get(eventOwner)->set(eventClassId, receiversFunctorMap);
	}

	EventFunctor<Event> eventFunctor;
	eventFunctor.setCallback(eventCallback);
	eventFunctor.mEventClassId = eventClassId;
	eventFunctor.mEventReceiver = eventReceiver;

	mOwnersReceiversMap->get(eventOwner)->get(eventClassId)->set(eventReceiver, eventFunctor);
}

void EventsManager::unsubscribe(ClassId eventClassId, DE_Class* eventOwner, DE_Class* eventReceiver){
	if(mOwnersReceiversMap->contains(eventOwner)){
		if(mOwnersReceiversMap->get(eventOwner)->contains(eventClassId)){
			if(mOwnersReceiversMap->get(eventOwner)->get(eventClassId)->contains(eventReceiver)){
				mOwnersReceiversMap->get(eventOwner)->get(eventClassId)->remove(eventReceiver);
			}
		}
	}
}

void EventsManager::send(DE_Class* eventOwner, DE_Class* eventInstigator, Event* event) {
	if(mOwnersReceiversMap->contains(eventOwner)){

		ClassId eventClassId = event->getClassId();
		if(mOwnersReceiversMap->get(eventOwner)->contains(eventClassId)){
			// Duplicate List. New event-receivers can subscribe during the iteration.
			// So we don't want to iterate a mutable list.
			List<EventFunctor<Event>>* eventFunctors = Memory::allocate<List<EventFunctor<Event>>>();
			eventFunctors->init(*mOwnersReceiversMap->get(eventOwner)->get(eventClassId)->getValues());

			FOR_LIST(it, mOwnersReceiversMap->get(eventOwner)->get(eventClassId)->getValues()){
				EventFunctor<Event> functor = it.get();
				functor.mEvent = event;
				functor.mEvent->mInstigator = eventInstigator;
				functor.execute();
			}

			Memory::free<List<EventFunctor<Event>>>(eventFunctors);
		}
	}
}

void EventsManager::terminate(){
	if(mOwnersReceiversMap){
		removeMapContent();
		mOwnersReceiversMap->clear();
	}
}

} /* namespace DE */
