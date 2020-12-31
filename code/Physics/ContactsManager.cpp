#include "ContactsManager.hpp"
#include "Physics/Collider.hpp"
#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"
#include "Log/Log.hpp"
#include "Memory/Memory.hpp"
#include "Scene/GameObject.hpp"
#include "Scripting/Script.hpp"
#include "Physics/RigidBody.hpp"
#include "Graphics/RenderEngine.hpp"

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

	contactPoint = Vector3(0, 0, 0);
	normal = Vector3(0, 0, 0);
	relativeVelocity = Vector3(0, 0, 0);

	status = Contact::ContactStatus::CONTACT_STATUS_ENTER;
}

void Contact::init(const Contact* otherContact) {
	colliderA = otherContact->colliderA;
	colliderB = otherContact->colliderB;

	contactPoint = otherContact->contactPoint;
	normal = otherContact->normal;
	relativeVelocity = otherContact->relativeVelocity;
}

// ---------------------------------------------------------------------------

ContactsManager::ContactsManager() : DE_Class(), Singleton() {
	mContactsMap = nullptr;
	mContactsToRemove = nullptr;
}

ContactsManager::~ContactsManager() {
	terminate();
	Memory::free<HashMap<Collider*, HashMap<Collider*, Contact*>*>>(mContactsMap);
	Memory::free<List<Contact*>>(mContactsToRemove);
}

// ---------------------------------------------------------------------------

void ContactsManager::removeContactFromMap(Contact* contact) {

	Collider *colliderA = contact->colliderA;
	Collider *colliderB = contact->colliderB;

	if (mContactsMap->contains(colliderA)) {
		if (mContactsMap->get(colliderA)->contains(colliderB)) {
			//contact = mContactsMap->get(colliderA)->get(colliderB);
			mContactsMap->get(colliderA)->remove(colliderB);
		}
	} else if (mContactsMap->contains(colliderB)) {
		if (mContactsMap->get(colliderB)->contains(colliderA)) {
			//contact = mContactsMap->get(colliderB)->get(colliderA);
			mContactsMap->get(colliderB)->remove(colliderA);
		}
	}

	//if (contact) {
		Memory::free<Contact>(contact);
	//}
}

// ---------------------------------------------------------------------------

void ContactsManager::resolveContact(Contact* contact) {

	Collider* colliderA = contact->colliderA;
	Collider* colliderB = contact->colliderB;
	GameObject* gameObjectA = colliderA->getGameObject();
	GameObject* gameObjectB = colliderB->getGameObject();
	/*ECHO("RESOLVE")
	ECHO("A - " + gameObjectA->getTag())
	ECHO("B - " + gameObjectB->getTag())*/

	Vector3 relativeVelocity = contact->relativeVelocity;

	Vector3 normal = contact->normal;

	f32 massA = colliderA->getRigidBody()->getMass();
	f32 massB = colliderB->getRigidBody()->getMass();

	f32 vrn = relativeVelocity.dot(normal);

	//VAR(f32, vrn);

	f32 j = (-(1+1.2f) * vrn) /
			(normal.dot(normal) * (1.0f/massA + 1.0f/massB));

	//resultLinear = resultLinear / 2.0f;

	if(vrn > 0){
		if(!colliderA->isStatic()){


			//ECHO("A - " + gameObjectA->getTag())
			Vector3 linear = colliderA->getRigidBody()->getLinear();
			//VAR(f32, linear.x)
			//VAR(f32, linear.y)
			//VAR(f32, linear.z)

			//RenderEngine::getInstance()->drawLine(colliderA->getCenter(), colliderA->getCenter() + (colliderA->getRigidBody()->getLinear().nor() * 100), 2.0f, true);

			if ((colliderA->getIsPenetrated())) {
				colliderA->getRigidBody()->restoreState();
				//it.get()->stopMovement();
				colliderA->unmarkPenetrated();
				colliderB->unmarkPenetrated();
			}

			colliderA->getRigidBody()->setLinear(colliderA->getRigidBody()->getLinear() - ((normal * j) / massA));
			colliderA->getRigidBody()->setLinear(colliderA->getRigidBody()->getLinear() / 2.0f);

			//ECHO("NEW LINEAR")
			linear = colliderA->getRigidBody()->getLinear();
			//VAR(f32, linear.x)
			//VAR(f32, linear.y)
			//VAR(f32, linear.z)

			//Vector3 linear(colliderA->getRigidBody()->getLinear());
			RenderEngine::getInstance()->drawLine(colliderA->getCenter(), colliderA->getCenter() + (linear.nor() * 100), 2.0f, true);
		}

		if(!colliderB->isStatic()){


			//ECHO("B - " + gameObjectB->getTag())
			Vector3 linear = Vector3(colliderB->getRigidBody()->getLinear());
			//VAR(f32, linear.x)
			//VAR(f32, linear.y)
			//VAR(f32, linear.z)

			//RenderEngine::getInstance()->drawLine(colliderB->getCenter(), colliderB->getCenter() + (Vector3(colliderB->getRigidBody()->getLinear()).nor() * 100), 2.0f, true);

			if ((colliderB->getIsPenetrated())) {
				colliderB->getRigidBody()->restoreState();
				//it.get()->stopMovement();
				colliderA->unmarkPenetrated();
				colliderB->unmarkPenetrated();
			}

			colliderB->getRigidBody()->setLinear(Vector3(colliderB->getRigidBody()->getLinear()) + ((normal * j) / massB));
			colliderB->getRigidBody()->setLinear(Vector3(colliderB->getRigidBody()->getLinear()) / 2.0f);

			//ECHO("NEW LINEAR")
			linear = Vector3(colliderB->getRigidBody()->getLinear());
			//VAR(f32, linear.x)
			//VAR(f32, linear.y)
			//VAR(f32, linear.z)

			//Vector3 linear(colliderB->getRigidBody()->getLinear());
			RenderEngine::getInstance()->drawLine(colliderB->getCenter(), colliderB->getCenter() + (linear.nor() * 100), 2.0f, true);
		}
	}

}

// ---------------------------------------------------------------------------

void ContactsManager::init() {
	DE_TRACE()

	mContactsMap = Memory::allocate<HashMap<Collider*, HashMap<Collider*, Contact*>*>>();
	mContactsMap->init();

	mContactsToRemove = Memory::allocate<List<Contact*>>();
	mContactsToRemove->init();
}

// ---------------------------------------------------------------------------

void ContactsManager::updateContacts() {
	FOR_LIST (itSubHashMaps, mContactsMap->getValues()) {
		FOR_LIST (itContacts, itSubHashMaps.get()->getValues()){
			Contact* contact = itContacts.get();

			Collider* colliderA = contact->colliderA;
			Collider* colliderB = contact->colliderB;

			if(colliderA->isActive() && colliderB->isActive()) {
				if (contact->status == Contact::ContactStatus::CONTACT_STATUS_ENTER) {
					GameObject* gameObjectA = colliderA->getGameObject();
					GameObject* gameObjectB = colliderB->getGameObject();

					resolveContact(contact);

					if (gameObjectA->isActive() && gameObjectA->getComponents<Script>() && gameObjectA->getComponents<Script>()->getLength() > 0) {
						Script* script = gameObjectA->getComponents<Script>()->get(0);
						script->onEnterCollision(gameObjectB);
					}

					if (gameObjectB->isActive() && gameObjectB->getComponents<Script>() && gameObjectB->getComponents<Script>()->getLength() > 0) {
						Script* script = gameObjectB->getComponents<Script>()->get(0);
						script->onEnterCollision(gameObjectA);
					}

					contact->status = Contact::ContactStatus::CONTACT_STATUS_UPDATE;

				} else if (contact->status == Contact::ContactStatus::CONTACT_STATUS_UPDATE) {
					GameObject* gameObjectA = colliderA->getGameObject();
					GameObject* gameObjectB = colliderB->getGameObject();

					ColliderStatus status = colliderA->testCollider(colliderB);

					if(status == ColliderStatus::STATUS_NONE){
						ColliderStatus status2 = colliderB->testCollider(colliderA);
						if(status < status2){
							status = status2;
						}
					}

					if(status == ColliderStatus::STATUS_NONE){
						removeContact(colliderA, colliderB);
					} else {
						if(colliderA->getIsPenetrated() || colliderB->getIsPenetrated()){
							resolveContact(contact);
						}

						/*RenderEngine::getInstance()->drawLine(colliderA->getCenter(), colliderA->getCenter() + (colliderA->getRigidBody()->getLinear().nor() * 100), 3.0f, true);
						RenderEngine::getInstance()->drawLine(colliderB->getCenter(), colliderB->getCenter() + (Vector3(colliderB->getRigidBody()->getLinear()).nor() * 100), 3.0f, true);*/

						if (gameObjectA->isActive() && gameObjectA->getComponents<Script>() && gameObjectA->getComponents<Script>()->getLength() > 0) {
							Script* script = gameObjectA->getComponents<Script>()->get(0);
							script->onCollision(gameObjectB);
						}

						if (gameObjectB->isActive() && gameObjectB->getComponents<Script>() && gameObjectB->getComponents<Script>()->getLength() > 0) {
							Script* script = gameObjectB->getComponents<Script>()->get(0);
							script->onCollision(gameObjectA);
						}
					}

				} else if (contact->status == Contact::ContactStatus::CONTACT_STATUS_EXIT) {
					GameObject* gameObjectA = colliderA->getGameObject();
					GameObject* gameObjectB = colliderB->getGameObject();

					if (gameObjectA->isActive() && gameObjectA->getComponents<Script>() && gameObjectA->getComponents<Script>()->getLength() > 0) {
						Script* script = gameObjectA->getComponents<Script>()->get(0);
						script->onExitCollision(gameObjectB);
					}

					if (gameObjectB->isActive() && gameObjectB->getComponents<Script>() && gameObjectB->getComponents<Script>()->getLength() > 0) {
						Script* script = gameObjectB->getComponents<Script>()->get(0);
						script->onExitCollision(gameObjectA);
					}

					//removeContactFromMap(colliderA, colliderB);

					mContactsToRemove->pushBack(contact);
				} else if (contact->status == Contact::ContactStatus::CONTACT_DESTROYED_COLLIDER) {
					mContactsToRemove->pushBack(contact);
				}
			}
		}
	}

	FOR_LIST(it, mContactsToRemove){
		removeContactFromMap(it.get());
	}

	mContactsToRemove->clear();
}

// ---------------------------------------------------------------------------

Contact* ContactsManager::addContact(Contact* newContact) {

	Contact* contact = findContact(newContact->colliderA, newContact->colliderB);

//	ECHO("TRY ADD CONTACT")
//	ECHO("A - " + newContact->colliderA->getGameObject()->getTag())
//	ECHO("B - " + newContact->colliderB->getGameObject()->getTag())

	if (!contact) {
//		ECHO("CONTACT ADDED!")
		contact = Memory::allocate<Contact>();
		contact->init(newContact);

		if (!mContactsMap->contains(contact->colliderA)) {
			auto subHashMap = Memory::allocate<HashMap<Collider*, Contact*>>();
			subHashMap->init();

			mContactsMap->set(contact->colliderA, subHashMap);
		}

		mContactsMap->get(contact->colliderA)->set(contact->colliderB, contact);
	}

	return contact;
}

// ---------------------------------------------------------------------------

void ContactsManager::removeContact(Collider *colliderA, Collider *colliderB) {

	GameObject* gameObjectA = colliderA->getGameObject();
	GameObject* gameObjectB = colliderB->getGameObject();
	//ECHO("REMOVE CONTACT")
	//ECHO("A - " + gameObjectA->getTag())
	//ECHO("B - " + gameObjectB->getTag())

	Contact* contact = findContact(colliderA, colliderB);

	if (contact) {
		contact->status = Contact::ContactStatus::CONTACT_STATUS_EXIT;
	}
}

// ---------------------------------------------------------------------------

Contact* ContactsManager::findContact(Collider *colliderA, Collider *colliderB) {
	Contact* contact = nullptr;

	if (mContactsMap->contains(colliderA)) {
		HashMap<Collider*, Contact*>* subHashMap = mContactsMap->get(colliderA);

		if (subHashMap->contains(colliderB)) {
			contact = subHashMap->get(colliderB);
		}
	} else if (mContactsMap->contains(colliderB)) {
		HashMap<Collider*, Contact*>* subHashMap = mContactsMap->get(colliderB);

		if (subHashMap->contains(colliderA)) {
			contact = subHashMap->get(colliderA);
		}
	}

	return contact;
}

// ---------------------------------------------------------------------------

void ContactsManager::notifyDestroyedCollider(Collider *collider) {

	FOR_LIST (itSubHashMaps, mContactsMap->getValues()) {
		FOR_LIST (itContacts, itSubHashMaps.get()->getValues()){
			Contact* contact = itContacts.get();

			if(contact->colliderA == collider || contact->colliderB == collider) {
				contact->status = Contact::ContactStatus::CONTACT_DESTROYED_COLLIDER;
			}
		}
	}
}


// ---------------------------------------------------------------------------

void ContactsManager::terminate() {
	if(mContactsMap){
		FOR_LIST (itSubHashMaps, mContactsMap->getValues()) {
			FOR_LIST (itContacts, itSubHashMaps.get()->getValues()) {
				Memory::free<Contact>(itContacts.get());
			}
			Memory::free<HashMap<Collider*, Contact*>>(itSubHashMaps.get());
		}
		mContactsMap->clear();
	}

	if(mContactsToRemove){
		FOR_LIST (it, mContactsToRemove) {
			Memory::free<Contact>(it.get());
		}
		mContactsToRemove->clear();
	}
}

} /* namespace DE */
