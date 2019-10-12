#ifndef DE_COLLIDER_H
#define DE_COLLIDER_H

#include "Component.h"
#include "Vector2.h"

namespace DE {

  template<class T> class Array;

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

public:

  DE_CLASS(Collider, Component);

  void init() override;

  void setSize(f32 width, f32 height);
  Array<Vector2>* getBoundingBox();
  f32 getRadius() const;

  bool checkCollisionRadius(Collider* otherCollider) const;

  ColliderStatus generateContacts(/*candidateVertices, otherCollider, contactManager*/);
  ColliderStatus testVertexVertex(/*candidateVertices, otherCollider, contactManager*/);
  ColliderStatus testVertexEdge(/*candidateVertices, otherCollider, contactManager*/);

  static bool testRectanglePoint(const Vector2& leftTop, f32 width, f32 height, const Vector2& point, f32 eps);
  static bool testSphereSphere(const Vector2& centerA, const Vector2& centerB, f32 radiusA, f32 radiusB);
};
} /* namespace DE */
#endif /* DE_COLLIDER_H */
