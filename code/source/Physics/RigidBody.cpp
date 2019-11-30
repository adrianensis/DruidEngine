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
}

// ---------------------------------------------------------------------------

RigidBody::RigidBody() : Component(){
  mLinear = Vector3(0,0,0);
  mForceAccumulator = Vector3(0,0,0);
  mMass = 1.0f;

  mState = State();

  mCollider = nullptr;
}

// ---------------------------------------------------------------------------

RigidBody::~RigidBody() = default;

// ---------------------------------------------------------------------------

void RigidBody::init(){
	TRACE();

  saveState();
}

// ---------------------------------------------------------------------------

void RigidBody::integrate(f32 deltaTime){

  mCollider = mCollider ? mCollider : getGameObject()->getComponents<Collider>()->get(0);

  Transform* t = getGameObject()->getTransform();

  // Symplectic Euler

  // v += (1/m * F) * dt
  mLinear.add(Vector3(mForceAccumulator).mul(1/mMass).mul(deltaTime));
  //mLinear.add(this.counterPenetrationAccumulator.cpy().mulScl(1/this.mass).mulScl(dt));

  // x += v * dt
  t->translate(Vector3(mLinear).mul(deltaTime));

  // clear forces
  mForceAccumulator.set(0,0,0);
}

// ---------------------------------------------------------------------------

void RigidBody::addForce(const Vector3& force) { mForceAccumulator.add(force); }

// ---------------------------------------------------------------------------

void RigidBody::stopMovement() { mForceAccumulator.set(0,0,0); mLinear.set(0,0,0); }

// ---------------------------------------------------------------------------

void RigidBody::saveState() {
  mState.mPosition = getGameObject()->getTransform()->getLocalPosition();
  mState.mLinear = mLinear;
  mState.mForceAccumulator = mForceAccumulator;
  mState.mMass = mMass;
}

void RigidBody::restoreState() {
  getGameObject()->getTransform()->setLocalPosition(mState.mPosition);
  mLinear = mState.mLinear;
  mForceAccumulator = mState.mForceAccumulator;
  mMass = mState.mMass;
}


} /* namespace DE */
