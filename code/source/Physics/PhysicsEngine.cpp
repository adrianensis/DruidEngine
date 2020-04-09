#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "QuadTree.h"
#include "GameObject.h"
#include "Memory.h"
#include "Time.h"
#include "List.h"
#include "Settings.h"

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

  mRigidBodies = Memory::allocate<List<RigidBody*>>();
  mRigidBodies->init();

  mQuadTree = Memory::allocate<QuadTree>();
  mQuadTree->init(sceneSize);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::step(f32 deltaTime){

  f32 dt = deltaTime;
  f32 maxIterations = 5.0f;

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

  mQuadTree->update();

  if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
    dt = dt/2.0f;
    bool tryAgain = true;
    u32 it = 0;

    while(tryAgain && it < maxIterations/*dt > minDeltaTime*/){

      tryAgain = false;

      FOR_LIST (it, mRigidBodies){
        if((it.get()->getCollider()->isPenetrated())){
          if(!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()){
            it.get()->restoreState();
            it.get()->integrate(dt);
          }
        }
      }

      mQuadTree->update();

      if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
        dt = dt/2.0f;
        tryAgain = true;
      }

      it++;
    }
  }

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

void PhysicsEngine::terminate(){
  TRACE();

  FOR_LIST(it, mRigidBodies){
    Memory::free<RigidBody>(it.get());
  }

  Memory::free<QuadTree>(mQuadTree);
}

// ---------------------------------------------------------------------------


} /* namespace DE */
