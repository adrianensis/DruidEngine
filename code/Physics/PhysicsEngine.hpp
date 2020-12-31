#ifndef DE_PHYSICSENGINE_H
#define DE_PHYSICSENGINE_H

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"

namespace DE {

class RigidBody;
class Collider;
class QuadTree;
class GameObject;
template<class T> class List;
class Iterator;

class PhysicsEngine: public DE_Class, public Singleton<PhysicsEngine> {
private:

	DE_M(RigidBodies, List<RigidBody*>*)
	DE_M(RigidBodiesToFree, List<RigidBody*>*)
	DE_M(QuadTree, QuadTree*)

	DE_M_GET_SET(DebugColliders, bool)

	void internalRemoveRigidBody(const Iterator *it);

public:

	DE_CLASS(PhysicsEngine, DE_Class)

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
