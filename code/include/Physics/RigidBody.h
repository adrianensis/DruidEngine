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

  Vector3 getLinear() const;
  Vector3 getForceAccumulator() const;
  f32 getMass() const;
  void setLinear(Vector3 linear);
  void addForce(const Vector3& force);
  void setMass(f32 mass);

};
} /* namespace DE */
#endif /* DE_RIGIDBODY_H */
