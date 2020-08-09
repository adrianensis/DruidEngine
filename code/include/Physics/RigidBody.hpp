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

	DE_CLASS(RigidBody, Component);

	void integrate(f32 deltaTime);

	void init() override;

	void stopMovement();

	void saveState();
	void restoreState();

	void addForce(const Vector3 &force) {
		mForceAccumulator.add(force);
	}
	Vector3 getForceAccumulator() const {
		return mForceAccumulator;
	};

	f32 getMass() const {
		return mMass;
	};

	void setMass(f32 newMass) {
		mMass = newMass;
	};

	void addLinear(const Vector3 &linear) {
		mLinear.add(linear);
	}
	Vector3 getLinear() const {
		return mLinear;
	};

	void setLinear(Vector3 newLinear) {
		mLinear = newLinear;
	};

	bool isSimulate() const {
		return mSimulate;
	};

	void setSimulate(bool newSimulate) {
		mSimulate = newSimulate;
	};

	Collider* getCollider() const {
		return mCollider;
	};

	void setCollider(Collider* collider) {
		mCollider = collider;
	}

	void addAntiPenetrationForce(Vector3 newAntiPenetrationForce) {
		mAntiPenetrationForce.add(newAntiPenetrationForce);
	};

	void resetAntiPenetrationForce() {
		mAntiPenetrationForce.set(0, 0, 0);
	};

};
} /* namespace DE */
#endif /* DE_RIGIDBODY_H */
