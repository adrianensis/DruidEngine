#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"

namespace DE {

// ---------------------------------------------------------------------------

RigidBody::RigidBody() : Component(){
  mLinear = Vector3(0,0,0);
  mForceAccumulator = Vector3(0,0,0);
  mMass = 1.0f;
}

// ---------------------------------------------------------------------------

RigidBody::~RigidBody() = default;

// ---------------------------------------------------------------------------

void RigidBody::init(){

}

// ---------------------------------------------------------------------------

void RigidBody::integrate(f32 deltaTime){
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

Vector3 RigidBody::getLinear() const { return mLinear; }
void RigidBody::setLinear(Vector3 linear) { mLinear = linear; }
Vector3 RigidBody::getForceAccumulator() const { return mForceAccumulator; }
void RigidBody::addForce(const Vector3& force) { mForceAccumulator.add(force); }
f32 RigidBody::getMass() const { return mMass; }
void RigidBody::setMass(f32 mass) { mMass = mass; }

} /* namespace DE */
