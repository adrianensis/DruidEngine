#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "List.h"

namespace DE {

// ---------------------------------------------------------------------------

RigidBody::State::State(){
  mPosition = Vector3(0,0,0);
  mLinear = Vector3(0,0,0);
  mForceAccumulator = Vector3(0,0,0);
  mMass = 0.0f;
  mSimulate = true;
}

// ---------------------------------------------------------------------------

RigidBody::RigidBody() : Component(){
  mLinear = Vector3(0,0,0);
  mForceAccumulator = Vector3(0,0,0);
  mMass = 1.0f;
  mSimulate = true;

  mState = State();

  mCollider = nullptr;
  mLastCollisionPosition = Vector3(0,0,0);

  mAntiPenetrationForce = Vector3(0,0,0);
}

// ---------------------------------------------------------------------------

RigidBody::~RigidBody() = default;

// ---------------------------------------------------------------------------

void RigidBody::init(){
	// TRACE();

  saveState();
}

// ---------------------------------------------------------------------------

void RigidBody::integrate(f32 deltaTime){

  if(mSimulate){

    Transform* t = getGameObject()->getTransform();

    // Symplectic Euler

    mForceAccumulator.add(mAntiPenetrationForce);

    // v += (1/m * F) * dt
    mLinear.add(Vector3(mForceAccumulator).mul(1.0f/mMass).mul(deltaTime));
    //mLinear.add(this.counterPenetrationAccumulator.cpy().mulScl(1/this.mass).mulScl(dt));

    // x += v * dt
    t->translate(Vector3(mLinear).mul(deltaTime));

    // clear forces
    mForceAccumulator.set(0,0,0);
  }
}

// ---------------------------------------------------------------------------

void RigidBody::stopMovement() { mForceAccumulator.set(0,0,0); mLinear.set(0,0,0); }

// ---------------------------------------------------------------------------

void RigidBody::saveState() {
  mState.mPosition = getGameObject()->getTransform()->getLocalPosition();
  mState.mLinear = mLinear;
  mState.mForceAccumulator = mForceAccumulator;
  mState.mMass = mMass;
  mState.mSimulate = mSimulate;
}

// ---------------------------------------------------------------------------

void RigidBody::restoreState() {
  getGameObject()->getTransform()->setLocalPosition(mState.mPosition);
  mLinear = mState.mLinear;
  mForceAccumulator = mState.mForceAccumulator;
  mMass = mState.mMass;
  mSimulate = mState.mSimulate;
}
// ---------------------------------------------------------------------------

Collider* RigidBody::initCollider() {
  mCollider = mCollider ? mCollider : getGameObject()->getComponents<Collider>()->get(0);
  return mCollider;
}


} /* namespace DE */
