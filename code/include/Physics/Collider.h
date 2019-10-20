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

public:

  DE_CLASS(Collider, Component);

  void init() override;

  void setSize(f32 width, f32 height);
  Array<Vector2>* getBoundingBox();
  Vector3 getRelativeVelocity(Collider* otherCollider);
  DE_GET(f32, Radius);
  DE_GET_SET(RigidBody*, RigidBody);

  bool checkCollisionRadius(Collider* otherCollider) const;

  ColliderStatus generateContacts(Array<Vector2>* candidateVertices, Collider* otherCollider/*, contactManager*/);
  ColliderStatus testVertexVertex(Array<Vector2>* candidateVertices, Collider* otherCollider/*, contactManager*/);
  ColliderStatus testVertexEdge(Array<Vector2>* candidateVertices, Collider* otherCollider/*, contactManager*/);

  ColliderStatus testRectangleRectangle(Collider* otherCollider);

  bool testPoint(Vector2 point);

  static bool testRectanglePoint(const Vector2& leftTop, f32 width, f32 height, const Vector2& point, f32 eps);
  static bool testSphereSphere(const Vector2& centerA, const Vector2& centerB, f32 radiusA, f32 radiusB);
};
} /* namespace DE */
#endif /* DE_COLLIDER_H */
