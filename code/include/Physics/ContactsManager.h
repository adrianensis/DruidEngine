#ifndef DE_CONTACTSMANAGER_H
#define DE_CONTACTSMANAGER_H

#include "DE_Class.h"
#include "Singleton.h"
#include "Vector3.h"

namespace DE {

	class Collider;
	template<class T> class List;
  template <class K, class V> class HashMap;

	class Contact : public DE_Class {
  public:

		enum class ContactStatus{
			CONTACT_STATUS_ENTER,
			CONTACT_STATUS_UPDATE,
			CONTACT_STATUS_EXIT
		};

		DE_CLASS(Contact, DE_Class);

    Collider* colliderA;
    Collider* colliderB;

    Vector3 contactPoint;
    Vector3 normal; // From B to A
    Vector3 relativeVelocity;

		ContactStatus status;

		void init();

		Contact& operator=(const Contact& rhs){
			if (this == &rhs) return *this; // handle self assignment

			colliderA = rhs.colliderA;
	    colliderB = rhs.colliderB;

	    contactPoint = rhs.contactPoint;
	    normal = rhs.normal;
	    relativeVelocity = rhs.relativeVelocity;

			return *this;
		}
  };

  class ContactsManager : public DE_Class, public Singleton<ContactsManager> {
	private:
		// ID = ContactID = ColliderA + Collider B
		HashMap<Collider*, HashMap<Collider*, Contact*>* >* mContactsMap;
		void removeContactFromMap(Collider* colliderA, Collider* colliderB);

	public:

		DE_CLASS(ContactsManager, DE_Class);

    // TODO : ID should be just the pointer or hash
		void init();
		void updateContacts();

		Contact* addContact(Collider* colliderA, Collider* colliderB);
		// void updateContact(Collider* colliderA, Collider* colliderB);
		void removeContact(Collider* colliderA, Collider* colliderB);
		Contact* findContact(Collider* colliderA, Collider* colliderB);
  };


} /* namespace DE */

#endif /* DE_CONTACTSMANAGER_H */
