#ifndef DE_PHYSICSENGINE_H
#define DE_PHYSICSENGINE_H

#include "DE_Class.hpp"
#include "Singleton.hpp"

namespace DE {

class RigidBody;
class Collider;
class QuadTree;
class GameObject;
template<class T> class List;
class Iterator;

class PhysicsEngine: public DE_Class, public Singleton<PhysicsEngine> {
private:

	List<RigidBody*>* mRigidBodies;
	List<RigidBody*>* mRigidBodiesToFree;
	QuadTree* mQuadTree;

	bool mDebugColliders;

	void internalRemoveRigidBody(const Iterator *it);

public:

	DE_CLASS(PhysicsEngine, DE_Class)

	DE_GET_SET(DebugColliders)

	void addRigidBody(RigidBody *rigidBody);
	void addCollider(RigidBody *rigidBody, Collider *collider);

	void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

	void init(f32 sceneSize);
	void step(f32 deltaTime);
	void updateContacts();

	void terminate();

};
} /* namespace DE */
#endif /* DE_PHYSICSENGINE_H */
