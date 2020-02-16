#include "Collider.h"
#include "Array.h"
#include "List.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Memory.h"
#include "Log.h"
#include "MathUtils.h"

namespace DE {

f32 Collider::msDepthEpsilon = 1.0f;

// ---------------------------------------------------------------------------

Collider::Collider() : Component(){
  mBoxVertices = nullptr;
  mWidth = 0.0f;
  mHalfWidth = 0.0f;
  mHeight = 0.0f;
  mHalfHeight = 0.0f;
  mRadius = 0.0f;
  mRigidBody = nullptr;
}

// ---------------------------------------------------------------------------

Collider::~Collider(){
  Memory::free<Array<Vector2>>(mBoxVertices);
}

// ---------------------------------------------------------------------------

void Collider::init(){
	TRACE();

  mBoxVertices = Memory::allocate<Array<Vector2>>();
  mBoxVertices->init(4);

  mBoxVertices->set(0, Vector2(0,0)); // LEFT TOP VERTEX
  mBoxVertices->set(1, Vector2(0,0)); // LEFT BOTTOM
  mBoxVertices->set(2, Vector2(0,0)); // RIGHT BOTTOM
  mBoxVertices->set(3, Vector2(0,0)); // RIGHT TOP

  mRigidBody = getGameObject()->getComponents<RigidBody>()->get(0);
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

Vector3 Collider::getRelativeVelocity(Collider* otherCollider){
  Vector3 velA = mRigidBody->getLinear();
	Vector3 velB = otherCollider->getRigidBody()->getLinear();

	return velA.sub(velB);
}

// ---------------------------------------------------------------------------

bool Collider::isSimulate(){
  return mRigidBody->isSimulate();
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testRectangleRectangle(Collider* otherCollider) {

  ColliderStatus result = ColliderStatus::STATUS_NONE;

  Transform* t = getGameObject()->getTransform();
  Vector3 center = t->getLocalPosition();

  Vector3 relativeVelocity = getRelativeVelocity(otherCollider);

  Vector3 normal = Vector3(center).sub(otherCollider->getGameObject()->getTransform()->getLocalPosition()).nor();
  f32 vrn = relativeVelocity.dot(normal);

  Array<Vector2>* otherVertices = otherCollider->getBoundingBox();

  FOR_ARRAY_COND(i, otherVertices, result != ColliderStatus::STATUS_PENETRATION) {
    ColliderStatus pointStatus = testPoint(otherVertices->get(i));

    if(pointStatus != ColliderStatus::STATUS_NONE){
      if(result != ColliderStatus::STATUS_PENETRATION){
        result = pointStatus;
      }
    }
  }

  if(result == ColliderStatus::STATUS_PENETRATION){
    ECHO("PENETRATION");
    if(vrn <= 0){
      //mRigidBody->setLinear(mRigidBody->getLinear() * -1.0f);
      //otherCollider->getRigidBody()->setLinear(otherCollider->getRigidBody()->getLinear() * -1.0f);
    }
  }else if(result == ColliderStatus::STATUS_COLLISION){
    ECHO("COLLISION");
    if(vrn <= 0){
      //mRigidBody->stopMovement();
      //otherCollider->getRigidBody()->stopMovement();
    }
  }

  setStatus(result);
  otherCollider->setStatus(result);

  return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::generateContacts(Array<Vector2>* candidateVertices, Collider* otherCollider/* contactManager*/) {

  ColliderStatus result = ColliderStatus::STATUS_NONE;

	// ColliderStatus resultVertexVertex = testVertexVertex(candidateVertices, otherCollider/*, contactManager*/);
  //
	// ColliderStatus resultVertexEdge = ColliderStatus::STATUS_NONE;
  //
	// // if penetration/collision has been detected in vertex-vertex phase, we don't need to check vertex-edge.
  //
	// if(resultVertexVertex == ColliderStatus::STATUS_NONE){
	// 	resultVertexEdge = testVertexEdge(candidateVertices, otherCollider/*, contactManager*/);
	// }
  //
	// // if one test has detected something.
	// if((resultVertexVertex != ColliderStatus::STATUS_NONE) || (resultVertexEdge != ColliderStatus::STATUS_NONE)){
  //
	//   bool hasInterpenetration = (resultVertexVertex == ColliderStatus::STATUS_PENETRATION) || (resultVertexEdge == ColliderStatus::STATUS_PENETRATION);
	// 	bool hasCollision = (resultVertexVertex == ColliderStatus::STATUS_COLLISION) || (resultVertexEdge == ColliderStatus::STATUS_COLLISION);
  //
	// 	if(hasInterpenetration){
	// 		result = ColliderStatus::STATUS_PENETRATION;
	// 	}else if(hasCollision){
	// 		result = ColliderStatus::STATUS_COLLISION;
	// 	}
	// }

	return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testVertexVertex(Array<Vector2>* candidateVertices, Collider* otherCollider/* contactManager*/) {

  ColliderStatus result = ColliderStatus::STATUS_NONE;

  // f32 eps = Collider::msDepthEpsilon; // Error
  //
	// Array<Vector2>* otherVertices = otherCollider->getBoundingBox();
  // //var normals = otherCollider.getNormals(); // the normals of the other collider
  //
	// // VERTEX - VERTEX
  //
	// bool foundVertexVertex = false; // true if d< eps
  //
  // f32 maxDistance = -9999999.0f; // distance = -INFINITY
  // Vector3 normal; // the collision normal
  // Vector3 selectedVertex;
  //
	// // var center = this.getCenter().cpy();
  //
  // // for all vertices
  // FOR_ARRAY_COND (i, candidateVertices, !foundVertexVertex) {
  //   Vector2 vertex = candidateVertices->get(i);
  //
  //   // flag interior vertex -> 1 , -1
  //   i32 interior = otherCollider->testPoint(vertex) ? -1 : 1;
  //
  //   //maxDistance = -9999999.0f; // distance
  //   //normal = null; // the collision normal
  //
	// 	// vertex - vertex
	//   FOR_ARRAY_COND (j, otherVertices, !foundVertexVertex) {
  //
	// 		Vector2 otherVertex = otherVertices->get(j);
  //
	//     f32 d = vertex.dst(otherVertex);
  //
  //     if(d < eps*1000){
  //       ECHO("d < eps*10");
  //       // this.pair.push(otherVertex);
  //
  //       // max
  //       if(d > maxDistance){
  //         ECHO("found VertexVertex");
  //         foundVertexVertex = true;
  //         // selectedVertex = vertex.cpy();
  //         // maxDistance = d;
	// 				// normal = center.sub(otherCollider.getCenter()).nor();
  //
  //       }
  //     }
  //   }
  // }
  //
  // if(foundVertexVertex){
  //
  //     //result = this.checkCollision(selectedVertex, eps, maxDistance, normal, otherCollider, contactManager);
  // }

  return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testVertexEdge(Array<Vector2>* candidateVertices, Collider* otherCollider/* contactManager*/) {
  return ColliderStatus::STATUS_NONE;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testPoint(Vector2 point) {

  //getBoundingBox(); // generate bounding box

  // if(this.LT === null){
  //   var center = this.getCenter();
  // 	this.LT = new Vector3(center.x-(this.width/2),center.y+(this.height/2), center.z);
  // }

  ColliderStatus result = ColliderStatus::STATUS_NONE;

	bool testDepthEpsilon = MathUtils::testRectanglePoint(mBoxVertices->get(0), mWidth, mHeight, point, msDepthEpsilon);

  if(testDepthEpsilon) {
    result = ColliderStatus::STATUS_COLLISION;

    bool testZeroDepthEpsilonEpsilon = MathUtils::testRectanglePoint(mBoxVertices->get(0), mWidth, mHeight, point, 0.0f);

    if(testZeroDepthEpsilonEpsilon){
      result = ColliderStatus::STATUS_PENETRATION;
    }
  }

  return result;
};

// ---------------------------------------------------------------------------

bool Collider::checkCollisionRadius(Collider* otherCollider) const {
  Vector2 thisPosition = Vector2(this->getGameObject()->getTransform()->getLocalPosition());
  Vector2 otherPosition = Vector2(otherCollider->getGameObject()->getTransform()->getLocalPosition());
  return MathUtils::testSphereSphere(thisPosition, otherPosition, getRadius(), otherCollider->getRadius());
}

// ---------------------------------------------------------------------------

} /* namespace DE */
