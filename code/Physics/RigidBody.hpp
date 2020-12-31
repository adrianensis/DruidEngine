#ifndef DE_RIGIDBODY_H
#define DE_RIGIDBODY_H

#include "Component.hpp"
#include "Vector3.hpp"

namespace DE {

class Collider;

class RigidBody: public Component {
private:

	class State {
	public:
		DE_M(Position, Vector3)
		DE_M(Linear, Vector3)
		DE_M(ForceAccumulator, Vector3)
		DE_M(Mass, f32)
		DE_M(Simulate, bool)

		State();
	};

	DE_M(State, State)
	DE_M(LastCollisionPosition, Vector3)
	DE_M(AntiPenetrationForce, Vector3)

	DE_M_GET(ForceAccumulator, Vector3)
	DE_M_GET_SET(Mass, f32)
	DE_M_GET_SET(Linear, Vector3)
	DE_M_GET_SET(Simulate, bool)
	DE_M_GET_SET(Collider, Collider*)

public:

	DE_CLASS(RigidBody, Component)

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
