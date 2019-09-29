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

PhysicsEngine::PhysicsEngine() : DE_Class(){
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
  mQuadTree->init(10000);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::step(){
  for (auto it = mRigidBodies->getIterator(); !it.isNull(); it.next()){
    //ECHO("ENGINE PHYSICS STEP");
    it.get()->integrate(Time::getDeltaTimeSeconds());
  }
}

// ---------------------------------------------------------------------------

void PhysicsEngine::terminate(){

}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------


} /* namespace DE */
