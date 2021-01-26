#include "Events/EventsManager.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

EventsManager::EventsManager() : DE_Class(), Singleton<EventsManager>() {
	mOwnersReceiversMap = nullptr;
}

EventsManager::~EventsManager() {
	if(mOwnersReceiversMap){
		removeMapContent();
		DE_FREE(mOwnersReceiversMap);
	}
}

void EventsManager::removeMapContent(){
	FOR_LIST(itEventReceivers, mOwnersReceiversMap->getValues()) {
		FOR_LIST(itFunctor, itEventReceivers.get()->getValues()) {
			DE_FREE(itFunctor.get());
		}

		DE_FREE(itEventReceivers.get());
	}
}

void EventsManager::init(){
	mOwnersReceiversMap = DE_NEW<OwnersReceiversMap>();
	mOwnersReceiversMap->init();
}

void EventsManager::subscribe(ClassId eventClassId, DE_Class* eventOwner, DE_Class* eventReceiver, EventCallback eventCallback){

	if(!mOwnersReceiversMap->contains(eventOwner)){
		EventReceiversMap* eventsReceiverMap = DE_NEW<EventReceiversMap>();
		eventsReceiverMap->init();
		mOwnersReceiversMap->set(eventOwner, eventsReceiverMap);
	}

	if(!mOwnersReceiversMap->get(eventOwner)->contains(eventClassId)){
		ReceiverFunctorsMap* receiversFunctorMap = DE_NEW<ReceiverFunctorsMap>();
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
			List<EventFunctor<Event>>* eventFunctors = DE_NEW<List<EventFunctor<Event>>>();
			eventFunctors->init(*mOwnersReceiversMap->get(eventOwner)->get(eventClassId)->getValues());

			FOR_LIST(it, mOwnersReceiversMap->get(eventOwner)->get(eventClassId)->getValues()){
				EventFunctor<Event> functor = it.get();
				functor.mEvent = event;
				functor.mEvent->mInstigator = eventInstigator;
				functor.execute();
			}

			DE_FREE(eventFunctors);
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
