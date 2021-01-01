#ifndef DE_CONTACTSMANAGER_H
#define DE_CONTACTSMANAGER_H

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Collider;
template<class T> class List;
template<class K, class V> class HashMap;

class Contact: public DE_Class {
public:

	enum class ContactStatus {
		CONTACT_STATUS_ENTER, CONTACT_STATUS_UPDATE, CONTACT_STATUS_EXIT, CONTACT_DESTROYED_COLLIDER
	};

	DE_CLASS(Contact)

	Collider* colliderA;
	Collider* colliderB;

	Vector3 contactPoint;
	Vector3 normal; // From B to A
	Vector3 relativeVelocity;

	ContactStatus status;

	void init();
	void init(const Contact* otherContact);
};

class ContactsManager: public DE_Class, public Singleton<ContactsManager> {
private:
	// ID = ContactID = ColliderA + Collider B
	using ContactsMap = HashMap<Collider*, HashMap<Collider*, Contact*>*>;
	DE_M(ContactsMap, ContactsMap*)
	DE_M(ContactsToRemove, List<Contact*>*)

	void removeContactFromMap(Contact* contact);
	void resolveContact(Contact* contact);

public:

	DE_CLASS(ContactsManager)

	// TODO : ID should be just the pointer or hash
	void init();
	void updateContacts();

	Contact* addContact(Contact* newContact);
	// void updateContact(Collider* colliderA, Collider* colliderB);
	void removeContact(Collider *colliderA, Collider *colliderB);
	Contact* findContact(Collider *colliderA, Collider *colliderB);
	void notifyDestroyedCollider(Collider *collider);

	void terminate();
};

} /* namespace DE */

#endif /* DE_CONTACTSMANAGER_H */
