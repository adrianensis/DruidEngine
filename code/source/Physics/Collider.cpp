#include "Collider.h"
#include "Array.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "Memory.h"
#include "Debug.h"

namespace DE {

// ---------------------------------------------------------------------------

Collider::Collider() : Component(){
  mBoxVertices = nullptr;
  mWidth = 0;
  mHalfWidth = 0;
  mHeight = 0;
  mHalfHeight = 0;
}

// ---------------------------------------------------------------------------

Collider::~Collider(){
  Memory::free<Array<Vector2>>(mBoxVertices);
}

// ---------------------------------------------------------------------------

void Collider::init(){
  mBoxVertices = Memory::allocate<Array<Vector2>>();
  mBoxVertices->init(4);

  mBoxVertices->set(0, Vector2(0,0)); // LEFT TOP VERTEX
  mBoxVertices->set(1, Vector2(0,0)); // LEFT BOTTOM
  mBoxVertices->set(2, Vector2(0,0)); // RIGHT BOTTOM
  mBoxVertices->set(3, Vector2(0,0)); // RIGHT TOP
}

// ---------------------------------------------------------------------------

void Collider::setSize(f32 width, f32 height){
  mWidth = width;
  mHalfWidth = width/2.0f;
  mHeight = height;
  mHalfHeight = height/2.0f;
}

// ---------------------------------------------------------------------------

Array<Vector2>* Collider::getBoundingBox() {

Transform* t = getGameObject()->getTransform();

bool isEmpty = mBoxVertices->getLength() == 0;

  if(t->isDirtyTranslation()){
    Vector3 center = t->getLocalPosition();

    mBoxVertices->set(0, Vector2(center.x - mHalfWidth, center.y + mHalfHeight)); // LEFT TOP VERTEX
    const Vector2& LeftTop = mBoxVertices->get(0);
    mBoxVertices->set(1, Vector2(LeftTop.x, LeftTop.y - mHeight)); // LEFT BOTTOM
    mBoxVertices->set(2, Vector2(LeftTop.x + mWidth, LeftTop.y - mHeight)); // RIGHT BOTTOM
    mBoxVertices->set(3, Vector2(LeftTop.x + mWidth, LeftTop.y)); // RIGHT TOP
  }

  return mBoxVertices;
};

// ---------------------------------------------------------------------------

bool Collider::testRectanglePoint(const Vector2& leftTop, f32 width, f32 height, const Vector2& point, f32 eps){
  // ECHO("testRectanglePoint");
  // VAR(f32, leftTop.x);
  // VAR(f32, leftTop.y);
  // VAR(f32, width);
  // VAR(f32, height);
  // VAR(f32, point.x);
  // VAR(f32, point.y);

  return (leftTop.x-eps < point.x && leftTop.y+eps > point.y &&
  leftTop.x + width +eps > point.x && leftTop.y - height -eps < point.y);
};

// ---------------------------------------------------------------------------


} /* namespace DE */
