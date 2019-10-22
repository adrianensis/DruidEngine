#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "QuadTree.h"
#include "GameObject.h"
#include "Memory.h"
#include "Time.h"
#include "List.h"

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

  List<Collider*>* ColliderList = rigidBody->getGameObject()->getComponents<Collider>();
  Collider* collider = ColliderList ? ColliderList->get(0) : nullptr;

  if(collider){
    mQuadTree->addCollider(collider);
  }

}

// ---------------------------------------------------------------------------

void PhysicsEngine::init(){
  mRigidBodies = Memory::allocate<List<RigidBody*>>();
  mRigidBodies->init();

  mQuadTree = Memory::allocate<QuadTree>();
  mQuadTree->init(2000);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::step(f32 deltaTime){

  f32 dt = deltaTime;

  f32 maxIterations = 5.0f;

  f32 minDeltaTime = deltaTime/maxIterations;
  bool tryAgain = true;

  FOR_LIST (it, mRigidBodies){
    it.get()->saveState();
  }

  bool firstIteration = true;

  while(tryAgain && dt > minDeltaTime){

    tryAgain = false;

    FOR_LIST (it, mRigidBodies){
      if(firstIteration || (it.get()->getCollider()->getStatus() == ColliderStatus::STATUS_PENETRATION)){
        it.get()->restoreState();
        it.get()->integrate(dt);
      }
    }

    mQuadTree->update();

    if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
      dt = dt/2.0f;
      tryAgain = true;
    }

    firstIteration = false;
  }

  if(mQuadTree->getStatus() == ColliderStatus::STATUS_PENETRATION){
    FOR_LIST (it, mRigidBodies){
      it.get()->restoreState();
    }
  }
}

// ---------------------------------------------------------------------------

void PhysicsEngine::terminate(){
  FOR_LIST(it, mRigidBodies){
    Memory::free<RigidBody>(it.get());
  }

  Memory::free<QuadTree>(mQuadTree);
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------


} /* namespace DE */
