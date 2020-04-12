#include "ContactsManager.h"
#include "Collider.h"
#include "List.h"
#include "HashMap.h"
#include "Log.h"
#include "Memory.h"
#include "GameObject.h"
#include "Script.h"

namespace DE {

// ---------------------------------------------------------------------------

Contact::Contact() : DE_Class() {
	init();
}

Contact::~Contact() {

}

void Contact::init() {
	colliderA = nullptr;
	colliderB = nullptr;

	contactPoint = Vector3(0,0,0);
	normal = Vector3(0,0,0);
	relativeVelocity = Vector3(0,0,0);

	status = Contact::ContactStatus::CONTACT_STATUS_ENTER;
}
// ---------------------------------------------------------------------------

ContactsManager::ContactsManager() : DE_Class(), Singleton() {
	mContactsMap = nullptr;
}

ContactsManager::~ContactsManager() {
	FOR_LIST (itSubHashMaps, mContactsMap->getValues()){
		FOR_LIST (itContacts, itSubHashMaps.get()->getValues()){
			Memory::free<Contact>(itContacts.get());
		}
		Memory::free<HashMap<Collider*, Contact*>>(itSubHashMaps.get());
	}
	Memory::free<HashMap<Collider*, HashMap<Collider*, Contact*>* >>(mContactsMap);
}

// ---------------------------------------------------------------------------

void ContactsManager::removeContactFromMap(Collider* colliderA, Collider* colliderB) {
	Contact* contact = nullptr;

	if(mContactsMap->contains(colliderA)){
		if(mContactsMap->get(colliderA)->contains(colliderB)){
			contact = mContactsMap->get(colliderA)->get(colliderB);
			mContactsMap->get(colliderA)->remove(colliderB);
		}
	} else if(mContactsMap->contains(colliderB)){
		if(mContactsMap->get(colliderB)->contains(colliderA)){
			contact = mContactsMap->get(colliderB)->get(colliderA);
			mContactsMap->get(colliderB)->remove(colliderA);
		}
	}

	if(contact){
		Memory::free<Contact>(contact);
	}
}

// ---------------------------------------------------------------------------

void ContactsManager::init() {
	TRACE();

	mContactsMap = Memory::allocate<HashMap<Collider*, HashMap<Collider*, Contact*>* >>();
	mContactsMap->init();
}

// ---------------------------------------------------------------------------

void ContactsManager::updateContacts() {
	FOR_LIST (itSubHashMaps, mContactsMap->getValues()){
		FOR_LIST (itContacts, itSubHashMaps.get()->getValues()){

			Contact* contact = itContacts.get();

			Collider* colliderA = contact->colliderA;
			Collider* colliderB = contact->colliderB;

			if(contact->status == Contact::ContactStatus::CONTACT_STATUS_ENTER){
				GameObject* gameObjectA = colliderA->getGameObject();
				GameObject* gameObjectB = colliderB->getGameObject();

				if(gameObjectA->getComponents<Script>() && gameObjectA->getComponents<Script>()->getLength() > 0){
					Script* script = gameObjectA->getComponents<Script>()->get(0);
					script->onEnterCollision(gameObjectB);
				}

				if(gameObjectB->getComponents<Script>() && gameObjectB->getComponents<Script>()->getLength() > 0){
					Script* script = gameObjectB->getComponents<Script>()->get(0);
					script->onEnterCollision(gameObjectA);
				}

				contact->status = Contact::ContactStatus::CONTACT_STATUS_UPDATE;

			}else if(contact->status == Contact::ContactStatus::CONTACT_STATUS_UPDATE){
				GameObject* gameObjectA = colliderA->getGameObject();
				GameObject* gameObjectB = colliderB->getGameObject();

				if(gameObjectA->getComponents<Script>() && gameObjectA->getComponents<Script>()->getLength() > 0){
					Script* script = gameObjectA->getComponents<Script>()->get(0);
					script->onCollision(gameObjectB);
				}

				if(gameObjectB->getComponents<Script>() && gameObjectB->getComponents<Script>()->getLength() > 0){
					Script* script = gameObjectB->getComponents<Script>()->get(0);
					script->onCollision(gameObjectA);
				}
			}else if(contact->status == Contact::ContactStatus::CONTACT_STATUS_EXIT){
				GameObject* gameObjectA = colliderA->getGameObject();
				GameObject* gameObjectB = colliderB->getGameObject();

				if(gameObjectA->getComponents<Script>() && gameObjectA->getComponents<Script>()->getLength() > 0){
					Script* script = gameObjectA->getComponents<Script>()->get(0);
					script->onExitCollision(gameObjectB);
				}

				if(gameObjectB->getComponents<Script>() && gameObjectB->getComponents<Script>()->getLength() > 0){
					Script* script = gameObjectB->getComponents<Script>()->get(0);
					script->onExitCollision(gameObjectA);
				}

				removeContactFromMap(colliderA, colliderB);
			}
		}
	}
}

// ---------------------------------------------------------------------------

Contact* ContactsManager::addContact(Collider* colliderA, Collider* colliderB) {
	Contact* contact = findContact(colliderA, colliderB);

	if(!contact) {
		Contact* newContact = Memory::allocate<Contact>();
		newContact->init();

		newContact->colliderA = colliderA;
		newContact->colliderB = colliderB;

		if(!mContactsMap->contains(colliderA)){
			auto subHashMap = Memory::allocate<HashMap<Collider*, Contact*>>();
			subHashMap->init();

			mContactsMap->set(colliderA, subHashMap);
		}

		mContactsMap->get(colliderA)->set(colliderB, newContact);
	}

	return contact;
}

// ---------------------------------------------------------------------------

// void ContactsManager::updateContact(Collider* colliderA, Collider* colliderB) {
//
// 	GameObject* gameObjectA = colliderA->getGameObject();
// 	GameObject* gameObjectB = colliderB->getGameObject();
//
// 	if(gameObjectA->getComponents<Script>() && gameObjectA->getComponents<Script>()->getLength() > 0){
// 		Script* script = gameObjectA->getComponents<Script>()->get(0);
// 		script->onCollision(gameObjectB);
// 	}
//
// 	if(gameObjectB->getComponents<Script>() && gameObjectB->getComponents<Script>()->getLength() > 0){
// 		Script* script = gameObjectB->getComponents<Script>()->get(0);
// 		script->onCollision(gameObjectA);
// 	}
//
// }

// ---------------------------------------------------------------------------

void ContactsManager::removeContact(Collider* colliderA, Collider* colliderB) {
	Contact* contact = findContact(colliderA, colliderB);

	if(contact){
		contact->status = Contact::ContactStatus::CONTACT_STATUS_EXIT;
	}
}

// ---------------------------------------------------------------------------

Contact* ContactsManager::findContact(Collider* colliderA, Collider* colliderB) {
	Contact* contact = nullptr;

	if(mContactsMap->contains(colliderA)){
		HashMap<Collider*, Contact*>* subHashMap = mContactsMap->get(colliderA);

		if(subHashMap->contains(colliderB)){
			contact = subHashMap->get(colliderB);
		}
	} else if(mContactsMap->contains(colliderB)){
		HashMap<Collider*, Contact*>* subHashMap = mContactsMap->get(colliderB);

		if(subHashMap->contains(colliderA)){
			contact = subHashMap->get(colliderA);
		}
	}

	return contact;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
