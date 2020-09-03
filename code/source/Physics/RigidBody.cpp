#include "RigidBody.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Collider.hpp"
#include "List.hpp"
#include "RenderEngine.hpp"
#include "Renderer.hpp"

namespace DE {

// ---------------------------------------------------------------------------

RigidBody::State::State() {
	mPosition = Vector3(0, 0, 0);
	mLinear = Vector3(0, 0, 0);
	mForceAccumulator = Vector3(0, 0, 0);
	mMass = 0.0f;
	mSimulate = true;
}

// ---------------------------------------------------------------------------

RigidBody::RigidBody() : Component() {
	mLinear = Vector3(0, 0, 0);
	mForceAccumulator = Vector3(0, 0, 0);
	mMass = 1.0f;
	mSimulate = true;

	mState = State();

	mCollider = nullptr;
	mLastCollisionPosition = Vector3(0, 0, 0);

	mAntiPenetrationForce = Vector3(0, 0, 0);
}

// ---------------------------------------------------------------------------

RigidBody::~RigidBody() {

}

// ---------------------------------------------------------------------------

void RigidBody::init() {
	// TRACE();

	saveState();
}

// ---------------------------------------------------------------------------

void RigidBody::integrate(f32 deltaTime) {

	if (mSimulate) {

		Transform* t = getGameObject()->getTransform();

		// Symplectic Euler

		mForceAccumulator.add(mAntiPenetrationForce);

		// v += (1/m * F) * dt
		mLinear.add(Vector3(mForceAccumulator).mul(1.0f / mMass).mul(deltaTime));

		//if(mCollider->isPenetrated()){
			mLinear.add(Vector3(mAntiPenetrationForce).mul(1/mMass).mul(deltaTime));
		//}

		Vector3 linear = mLinear;

		//RenderEngine::getInstance()->drawLine(getCollider()->getCenter(), getCollider()->getCenter() + (linear.nor() * 50), 1.0f, true);

		// x += v * dt
		t->translate(Vector3(mLinear).mul(deltaTime));

		// clear forces
		mForceAccumulator.set(0, 0, 0);
		//mAntiPenetrationForce.set(0, 0, 0);

		if(isSleeping()){
			mLinear.set(0, 0, 0);
			//getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(-1,0,1,1));
		}
	}
}

// ---------------------------------------------------------------------------

void RigidBody::stopMovement() {
	mForceAccumulator.set(0, 0, 0);
	mLinear.set(0, 0, 0);
}

// ---------------------------------------------------------------------------

void RigidBody::saveState() {
	mState.mPosition = getGameObject()->getTransform()->getLocalPosition();
//	mState.mLinear = mLinear;
//	mState.mForceAccumulator = mForceAccumulator;
//	mState.mMass = mMass;
//	mState.mSimulate = mSimulate;
}

// ---------------------------------------------------------------------------

void RigidBody::restoreState() {
	getGameObject()->getTransform()->setLocalPosition(mState.mPosition);
//	mLinear = mState.mLinear;
//	mForceAccumulator = mState.mForceAccumulator;
//	mMass = mState.mMass;
//	mSimulate = mState.mSimulate;
}

// ---------------------------------------------------------------------------

bool RigidBody::isSleeping() {
	return mLinear.len() < 15.0f;
}

} /* namespace DE */
