#include "PhysicsEngine.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "QuadTree.hpp"
#include "GameObject.hpp"
#include "Memory.hpp"
#include "Time2.hpp"
#include "List.hpp"
#include "Settings.hpp"
#include "ContactsManager.hpp"
#include "RenderEngine.hpp"
#include "Renderer.hpp"

namespace DE {

// ---------------------------------------------------------------------------

PhysicsEngine::PhysicsEngine() : DE_Class(), Singleton() {
	mRigidBodies = nullptr;
	mQuadTree = nullptr;
}

// ---------------------------------------------------------------------------

PhysicsEngine::~PhysicsEngine() = default;

// ---------------------------------------------------------------------------

void PhysicsEngine::addRigidBody(RigidBody *rigidBody) {
	mRigidBodies->pushBack(rigidBody);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::addCollider(RigidBody *rigidBody, Collider *collider) {
	rigidBody->setCollider(collider);
	mQuadTree->addCollider(collider);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::rayCastQuery(const Vector3 &lineStart, const Vector3 &lineEnd, List<GameObject*> *outList) {
	RenderEngine::getInstance()->drawLine(lineStart, lineEnd);
	mQuadTree->rayCastQuery(lineStart, lineEnd, outList);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::internalRemoveRigidBody(const Iterator *it) {
	auto castedIt = it->cast<RigidBody*>();
	mRigidBodies->remove(*castedIt);

	RigidBody* rigidBody = (*castedIt).get();
	rigidBody->setDestroyed();
	Memory::free<RigidBody>(rigidBody);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::init(f32 sceneSize) {
	TRACE();

	ContactsManager::getInstance()->init();

	mRigidBodies = Memory::allocate<List<RigidBody*>>();
	mRigidBodies->init();

	mQuadTree = Memory::allocate<QuadTree>();
	mQuadTree->init(sceneSize);

}

// ---------------------------------------------------------------------------

void PhysicsEngine::step(f32 deltaTime) {

	u32 it = 0;
	u32 maxIterations = 5.0f; // how many times we want to divide dt
	f32 factor = 3.0f; // how dt is divided

	f32 minDeltaTime = deltaTime / (maxIterations*factor);

	f32 dt = deltaTime;

	//bool tryAgain = true;

	//while(tryAgain && dt > minDeltaTime && it < maxIterations){
		FOR_LIST (it, mRigidBodies) {
			if (it.get()->isActive()) {
				it.get()->getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
				if (!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()) {
//					if (!it.get()->getCollider()->isPenetrated()) {
							it.get()->saveState();
							it.get()->integrate(dt);
//					} else {
						//it.get()->restoreState();
						//it.get()->integrate(-dt*2);
//						it.get()->getCollider()->unmarkPenetrated();
//					}
				}
			} else if (it.get()->isPendingToBeDestroyed()) {
				internalRemoveRigidBody(&it);
			}
		}

		//tryAgain = false;

		mQuadTree->update();

		/*if (mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION) {

			FOR_LIST (it, mRigidBodies) {
				if (!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()) {
					if ((it.get()->getCollider()->isPenetrated())) {
						it.get()->restoreState();
						//it.get()->integrate(-dt);
						//it.get()->stopMovement();
					}

					it.get()->getCollider()->unmarkPenetrated();
				}
			}

			//dt = dt / factor;
			//tryAgain = true;
		}*/

//		FOR_LIST (it, mRigidBodies) {
//			if ((it.get()->getCollider()->isPenetrated())) {
//
//				if (!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()) {
//					it.get()->restoreState();
//					//it.get()->stopMovement();
//				}
//
//				it.get()->getCollider()->unmarkPenetrated();
//			}
//		}

		mQuadTree->setStatus(ColliderStatus::STATUS_NONE); // Reset status and try again.

		++it;

		updateContacts();
	//}

	//ECHO("------------------------")
}

// ---------------------------------------------------------------------------

void PhysicsEngine::updateContacts() {
	ContactsManager::getInstance()->updateContacts();
}

// ---------------------------------------------------------------------------

void PhysicsEngine::terminate() {
	TRACE();

	FOR_LIST(it, mRigidBodies) {
		Memory::free<RigidBody>(it.get());
	}

	Memory::free<QuadTree>(mQuadTree);

	Memory::free<ContactsManager>(ContactsManager::getInstance());
}

// ---------------------------------------------------------------------------

} /* namespace DE */
