#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

namespace DE {

class RigidBody;
class Collider;
class QuadTree;
class GameObject;
template<class T> class List;
class Iterator;

class PhysicsEngine: public ObjectBase, public Singleton<PhysicsEngine> {
private:

	 List<RigidBody*>* mRigidBodies;
	 List<RigidBody*>* mRigidBodiesToFree;
	 QuadTree* mQuadTree;

	 bool mDebugColliders;

	void internalRemoveRigidBody(const Iterator *it);

public:

	GENERATE_METADATA(PhysicsEngine);

	PhysicsEngine();
	virtual ~PhysicsEngine() override;;

	GET_SET(DebugColliders);

	void addRigidBody(RigidBody *rigidBody);
	void addCollider(RigidBody *rigidBody, Collider *collider);

	void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

	void init(f32 sceneSize);
	void step(f32 deltaTime);
	void updateContacts();

	void terminate();

};
}

