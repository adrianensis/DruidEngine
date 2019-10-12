#include "Collider.h"
#include "Array.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Memory.h"
#include "Debug.h"
#include "MathUtils.h"

namespace DE {

// ---------------------------------------------------------------------------

Collider::Collider() : Component(){
  mBoxVertices = nullptr;
  mWidth = 0.0f;
  mHalfWidth = 0.0f;
  mHeight = 0.0f;
  mHalfHeight = 0.0f;
  mRadius = 0.0f;
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

  mRadius = sqrt(( mWidth * mWidth ) + (  mHeight * mHeight )) / 2.0f;
}

// ---------------------------------------------------------------------------

Array<Vector2>* Collider::getBoundingBox() {

  Transform* t = getGameObject()->getTransform();

  if(t->isDirtyTranslation()){
    Vector3 center = t->getLocalPosition();

    mBoxVertices->set(0, Vector2(center.x - mHalfWidth, center.y + mHalfHeight)); // LEFT TOP VERTEX
    const Vector2& LeftTop = mBoxVertices->get(0);
    mBoxVertices->set(1, Vector2(LeftTop.x, LeftTop.y - mHeight)); // LEFT BOTTOM
    mBoxVertices->set(2, Vector2(LeftTop.x + mWidth, LeftTop.y - mHeight)); // RIGHT BOTTOM
    mBoxVertices->set(3, Vector2(LeftTop.x + mWidth, LeftTop.y)); // RIGHT TOP
  }

  return mBoxVertices;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::generateContacts(/*candidateVertices, otherCollider, contactManager*/) {

  ColliderStatus result = ColliderStatus::STATUS_NONE;

	ColliderStatus resultVertexVertex = testVertexVertex(/*candidateVertices, otherCollider, contactManager*/);

	ColliderStatus resultVertexEdge = ColliderStatus::STATUS_NONE;

	// if penetration/collision has been detected in vertex-vertex phase, we don't need to check vertex-edge.

	if(resultVertexVertex == ColliderStatus::STATUS_NONE){
		resultVertexEdge = testVertexEdge(/*candidateVertices, otherCollider, contactManager*/);
	}

	// if one test has detected something.
	if((resultVertexVertex != ColliderStatus::STATUS_NONE) || (resultVertexEdge != ColliderStatus::STATUS_NONE)){

	  bool hasInterpenetration = (resultVertexVertex == ColliderStatus::STATUS_PENETRATION) || (resultVertexEdge == ColliderStatus::STATUS_PENETRATION);
		bool hasCollision = (resultVertexVertex == ColliderStatus::STATUS_COLLISION) || (resultVertexEdge == ColliderStatus::STATUS_COLLISION);

		if(hasInterpenetration){
			result = ColliderStatus::STATUS_PENETRATION;
		}else if(hasCollision){
			result = ColliderStatus::STATUS_COLLISION;
		}
	}

	return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testVertexVertex(/*candidateVertices, otherCollider, contactManager*/) {
  return ColliderStatus::STATUS_NONE;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testVertexEdge(/*candidateVertices, otherCollider, contactManager*/) {
  return ColliderStatus::STATUS_NONE;
}

// ---------------------------------------------------------------------------

f32 Collider::getRadius() const { return mRadius; }

// ---------------------------------------------------------------------------

bool Collider::checkCollisionRadius(Collider* otherCollider) const {
  Vector2 thisPosition = Vector2(this->getGameObject()->getTransform()->getLocalPosition());
  Vector2 otherPosition = Vector2(otherCollider->getGameObject()->getTransform()->getLocalPosition());
  return testSphereSphere(thisPosition, otherPosition, getRadius(), otherCollider->getRadius());
}

// ---------------------------------------------------------------------------

bool Collider::testRectanglePoint(const Vector2& leftTop, f32 width, f32 height, const Vector2& point, f32 eps) {
  // ECHO("testRectanglePoint");
  // VAR(f32, leftTop.x);
  // VAR(f32, leftTop.y);
  // VAR(f32, width);
  // VAR(f32, height);
  // VAR(f32, point.x);
  // VAR(f32, point.y);

  return (leftTop.x-eps < point.x && leftTop.y+eps > point.y &&
          leftTop.x + width +eps > point.x && leftTop.y - height -eps < point.y);
}

// ---------------------------------------------------------------------------

bool Collider::testSphereSphere(const Vector2 &centerA, const Vector2 &centerB, f32 radiusA, f32 radiusB) {
  f32 distance = centerA.dst(centerB);
  return (distance < radiusA+radiusB);
}

// ---------------------------------------------------------------------------


} /* namespace DE */
