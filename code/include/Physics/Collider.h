#ifndef DE_COLLIDER_H
#define DE_COLLIDER_H

#include "Component.h"
#include "Vector2.h"

namespace DE {

  template<class T> class Array;
  class RigidBody;

enum class ColliderStatus
{
  STATUS_NONE,
  STATUS_PENETRATION,
  STATUS_COLLISION
};

class Collider : public Component{
private:

  Array<Vector2>* mBoxVertices;

  f32 mWidth;
  f32 mHalfWidth;
  f32 mHeight;
  f32 mHalfHeight;
  f32 mRadius;

  static f32 msDepthEpsilon;

  RigidBody* mRigidBody;

  ColliderStatus mStatus;

public:

  DE_CLASS(Collider, Component);

  void init() override;

  void setSize(f32 width, f32 height);
  Array<Vector2>* getBoundingBox();
  Vector3 getRelativeVelocity(Collider* otherCollider);
  DE_GET(f32, Radius);
  DE_GET_SET(RigidBody*, RigidBody);
  DE_GET_SET(ColliderStatus, Status);

  bool getSimulate();

  bool checkCollisionRadius(Collider* otherCollider) const;

  ColliderStatus generateContacts(Array<Vector2>* candidateVertices, Collider* otherCollider/*, contactManager*/);
  ColliderStatus testVertexVertex(Array<Vector2>* candidateVertices, Collider* otherCollider/*, contactManager*/);
  ColliderStatus testVertexEdge(Array<Vector2>* candidateVertices, Collider* otherCollider/*, contactManager*/);

  ColliderStatus testRectangleRectangle(Collider* otherCollider);

  ColliderStatus testPoint(Vector2 point);
};
} /* namespace DE */
#endif /* DE_COLLIDER_H */
