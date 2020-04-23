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

namespace DE {

// ---------------------------------------------------------------------------

PhysicsEngine::PhysicsEngine() : DE_Class(), Singleton(){
  mRigidBodies = nullptr;
  mQuadTree = nullptr;
}

// ---------------------------------------------------------------------------

PhysicsEngine::~PhysicsEngine() = default;

// ---------------------------------------------------------------------------

void PhysicsEngine::addRigidBody(RigidBody* rigidBody){
  mRigidBodies->pushBack(rigidBody);

  Collider* collider = rigidBody->initCollider();

  if(collider){
    mQuadTree->addCollider(collider);
  }
}

// ---------------------------------------------------------------------------

void PhysicsEngine::rayCastQuery(const Vector3& lineStart, const Vector3& lineEnd, List<GameObject*>* outList){
  RenderEngine::getInstance()->drawLine(lineStart, lineEnd);
  mQuadTree->rayCastQuery(lineStart, lineEnd, outList);
}


// ---------------------------------------------------------------------------

void PhysicsEngine::internalRemoveRigidBody(const Iterator* it){
	auto castedIt = it->cast<RigidBody*>();
	mRigidBodies->remove(*castedIt);

  RigidBody* rigidBody = (*castedIt).get();
  rigidBody->setDestroyed();
	Memory::free<RigidBody>(rigidBody);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::init(f32 sceneSize){
	TRACE();

  ContactsManager::getInstance()->init();

  mRigidBodies = Memory::allocate<List<RigidBody*>>();
  mRigidBodies->init();

  mQuadTree = Memory::allocate<QuadTree>();
  mQuadTree->init(sceneSize);

}

// ---------------------------------------------------------------------------

void PhysicsEngine::step(f32 deltaTime){

  f32 dt = deltaTime;
  f32 maxIterations = 5.0f;

  // ECHO("INTEGRATE")

  FOR_LIST (it, mRigidBodies){
    if(it.get()->isActive()){
      if( ! it.get()->getCollider()->isPenetrated()){
        if(!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()){
          it.get()->saveState();
          it.get()->integrate(dt);
        }
      }
    } else if(it.get()->isPendingToBeDestroyed()){
      internalRemoveRigidBody(&it);
    }
  }

  // ECHO("UPDATE")
  mQuadTree->update();

  // if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
  //   dt = dt/2.0f;
  //   bool tryAgain = true;
  //   u32 it = 0;
  //
  //   while(tryAgain && it < maxIterations/*dt > minDeltaTime*/){
  //
  //     tryAgain = false;
  //
  //     FOR_LIST (it, mRigidBodies){
  //       if((it.get()->getCollider()->isPenetrated())){
  //         if(!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()){
  //           it.get()->restoreState();
  //           it.get()->integrate(dt);
  //         }
  //       }
  //     }
  //
  //     // ECHO("UPDATE BC PENETRATION")
  //     mQuadTree->update();
  //
  //     if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
  //       dt = dt/2.0f;
  //       tryAgain = true;
  //     }
  //
  //     it++;
  //   }
  // }

  // if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
  //   ECHO("PENETRATION");
  // }
  // else if(mQuadTree->getStatus() == ColliderStatus::STATUS_COLLISION){
  //   ECHO("COLLISION");
  // }
  // else if(mQuadTree->getStatus() == ColliderStatus::STATUS_NONE){
  //   ECHO("NONE");
  // }

  if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
    // ECHO("RESTORE")

    FOR_LIST (it, mRigidBodies){
      if((it.get()->getCollider()->isPenetrated()/*getStatus() == ColliderStatus::STATUS_PENETRATION*/)){

        if(!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()){
          it.get()->restoreState();
          it.get()->stopMovement();
        }
      }

      it.get()->getCollider()->unmarkPenetrated();
    }
  }

  mQuadTree->setStatus(ColliderStatus::STATUS_NONE); // Reset status and try again.
}

// ---------------------------------------------------------------------------

void PhysicsEngine::updateContacts(){
  // TRACE();
  ContactsManager::getInstance()->updateContacts();
}
// ---------------------------------------------------------------------------

void PhysicsEngine::terminate(){
  TRACE();

  FOR_LIST(it, mRigidBodies){
    Memory::free<RigidBody>(it.get());
  }

  Memory::free<QuadTree>(mQuadTree);

  Memory::free<ContactsManager>(ContactsManager::getInstance());
}

// ---------------------------------------------------------------------------


} /* namespace DE */
