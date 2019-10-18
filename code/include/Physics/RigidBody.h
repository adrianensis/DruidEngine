#ifndef DE_RIGIDBODY_H
#define DE_RIGIDBODY_H

#include "Component.h"
#include "Vector3.h"

namespace DE {

class RigidBody : public Component{
private:

  Vector3 mLinear;
  Vector3 mForceAccumulator;
  f32 mMass;

public:

  DE_CLASS(RigidBody, Component);

  void integrate(f32 deltaTime);

  void init() override;

  void addForce(const Vector3& force);
  
  DE_GET(Vector3, ForceAccumulator);

  DE_GET_SET(f32, Mass);
  DE_GET_SET(Vector3, Linear);
};
} /* namespace DE */
#endif /* DE_RIGIDBODY_H */
