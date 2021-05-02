#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Collider;

class RigidBody : public Component {
private:

	class State {
	public:
		 Vector3 mPosition;
		 Vector3 mLinear;
		 Vector3 mForceAccumulator;
		 f32 mMass;
		 bool mSimulate;

		State();
	};

	 State mState;
	 Vector3 mLastCollisionPosition;
	 Vector3 mAntiPenetrationForce;

	 Vector3 mForceAccumulator;
	 f32 mMass;
	 Vector3 mLinear;
	 bool mSimulate;
	 Collider* mCollider;

public:

	GENERATE_METADATA(RigidBody);

	RigidBody();
	virtual ~RigidBody() override;;

	GET(ForceAccumulator);
	GET_SET(Mass);
	GET_SET(Linear);
	GET_SET(Simulate);
	GET_SET(Collider);

	void init() override;

	void integrate(f32 deltaTime);
	void stopMovement();

	void saveState();
	void restoreState();

	void addForce(const Vector3 &force) {
		mForceAccumulator.add(force);
	}

	void addLinear(const Vector3 &linear) {
		mLinear.add(linear);
	}

	void addAntiPenetrationForce(Vector3 newAntiPenetrationForce) {
		mAntiPenetrationForce.add(newAntiPenetrationForce);
	};

	void resetAntiPenetrationForce() {
		mAntiPenetrationForce.set(0, 0, 0);
	};

	bool isSleeping();

};
}

