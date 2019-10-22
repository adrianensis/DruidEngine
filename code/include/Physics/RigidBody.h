#ifndef DE_RIGIDBODY_H
#define DE_RIGIDBODY_H

#include "Component.h"
#include "Vector3.h"

namespace DE {

  class Collider;

class RigidBody : public Component{
private:

  Vector3 mLinear;
  Vector3 mForceAccumulator;
  f32 mMass;

  class State {
  public:
    Vector3 mPosition;
    Vector3 mLinear;
    Vector3 mForceAccumulator;
    f32 mMass;

    State();
  };

  State mState;

  Collider* mCollider;

public:

  DE_CLASS(RigidBody, Component);

  void integrate(f32 deltaTime);

  void init() override;

  void addForce(const Vector3& force);

  void stopMovement();

  void saveState();
  void restoreState();

  DE_GET(Vector3, ForceAccumulator);

  DE_GET_SET(f32, Mass);
  DE_GET_SET(Vector3, Linear);
  
  DE_GET(Collider*, Collider);
};
} /* namespace DE */
#endif /* DE_RIGIDBODY_H */
