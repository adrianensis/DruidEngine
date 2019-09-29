#ifndef DE_COLLIDER_H
#define DE_COLLIDER_H

#include "Component.h"
#include "Vector2.h"

namespace DE {

  template<class T> class Array;

class Collider : public Component{
private:

  Array<Vector2>* mBoxVertices;

  f32 mWidth;
  f32 mHalfWidth;
  f32 mHeight;
  f32 mHalfHeight;

public:

  DE_CLASS(Collider, Component);

  void init() override;

  void setSize(f32 width, f32 height);
  Array<Vector2>* getBoundingBox();

  static bool testRectanglePoint(const Vector2& leftTop, f32 width, f32 height, const Vector2& point, f32 eps);

};
} /* namespace DE */
#endif /* DE_COLLIDER_H */
