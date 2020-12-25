#ifndef DE_RIGIDBODY_H
#define DE_RIGIDBODY_H

#include "Component.hpp"
#include "Vector3.hpp"

namespace DE {

class Collider;

class RigidBody: public Component {
private:

	Vector3 mLinear;
	Vector3 mForceAccumulator;
	f32 mMass;
	bool mSimulate;

	class State {
	public: Vector3 mPosition;
		Vector3 mLinear;
		Vector3 mForceAccumulator;
		f32 mMass;
		bool mSimulate;

		State();
	};

	State mState;

	Collider* mCollider;
	Vector3 mLastCollisionPosition;

	Vector3 mAntiPenetrationForce;

public:

	DE_CLASS(RigidBody, Component)

	DE_GET(ForceAccumulator)
	DE_GET_SET(Mass)
	DE_GET_SET(Linear)
	DE_GET_SET(Simulate)
	DE_GET_SET(Collider)

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
} /* namespace DE */
#endif /* DE_RIGIDBODY_H */
