#include "Collider.hpp"
#include "Array.hpp"
#include "List.hpp"
#include "RenderEngine.hpp"
#include "GameObject.hpp"
#include "RigidBody.hpp"
#include "Transform.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Memory.hpp"
#include "Log.hpp"
#include "MathUtils.hpp"
#include "ContactsManager.hpp"
#include "Renderer.hpp"

namespace DE {

f32 Collider::msDepthEpsilon = 10.0f;

// ---------------------------------------------------------------------------

Collider::Collider() : Component() {
	mBoxVertices = nullptr;
	mWidth = 0.0f;
	mHalfWidth = 0.0f;
	mHeight = 0.0f;
	mHalfHeight = 0.0f;
	mRadius = 0.0f;
	mRigidBody = nullptr;
	mIsPenetrated = false;
	mIsSolid = true;
	mColliderShape = ColliderShape::RECTANGLE;
	mLastContact = nullptr;
}

// ---------------------------------------------------------------------------

Collider::~Collider() {
	if (mBoxVertices) {
		Memory::free<Array<Vector2>>(mBoxVertices);
	}

	Memory::free<Contact>(mLastContact);
}

// ---------------------------------------------------------------------------

void Collider::init() {
	// TRACE();

	mBoxVertices = Memory::allocate<Array<Vector2>>();
	mBoxVertices->init(4);

	mBoxVertices->set(0, Vector2(0, 0)); // LEFT TOP VERTEX
	mBoxVertices->set(1, Vector2(0, 0)); // LEFT BOTTOM
	mBoxVertices->set(2, Vector2(0, 0)); // RIGHT BOTTOM
	mBoxVertices->set(3, Vector2(0, 0)); // RIGHT TOP

	mRigidBody = getGameObject()->getComponents<RigidBody>()->get(0);

	mPositionOffset.set(0, 0, 0);

	mCollisionLayer = 0;

	mLastContact = Memory::allocate<Contact>();
	mLastContact->init();
}

// ---------------------------------------------------------------------------

void Collider::setSize(f32 width, f32 height) {
	mWidth = width;
	mHalfWidth = width / 2.0f;
	mHeight = height;
	mHalfHeight = height / 2.0f;

	mRadius = sqrt((mWidth * mWidth) + (mHeight * mHeight)) / 2.0f;

	getBoundingBox(true);
}

// ---------------------------------------------------------------------------

Array<Vector2>* Collider::getBoundingBox(bool forceCalculateBoundingBox/* = false*/) {

	Transform* t = getGameObject()->getTransform();

	if (t->isDirtyTranslation() || forceCalculateBoundingBox) {
		Vector3 center = getCenter();

		mBoxVertices->set(0, Vector2(center.x - mHalfWidth, center.y + mHalfHeight)); // LEFT TOP VERTEX
		const Vector2& LeftTop = mBoxVertices->get(0);
		mBoxVertices->set(1, Vector2(LeftTop.x, LeftTop.y - mHeight)); // LEFT BOTTOM
		mBoxVertices->set(2, Vector2(LeftTop.x + mWidth, LeftTop.y - mHeight)); // RIGHT BOTTOM
		mBoxVertices->set(3, Vector2(LeftTop.x + mWidth, LeftTop.y)); // RIGHT TOP
	}

	return mBoxVertices;
}

// ---------------------------------------------------------------------------

Vector3 Collider::getCenter() const {
	return getGameObject()->getTransform()->getWorldPosition() + mPositionOffset;
}

// ---------------------------------------------------------------------------

Vector3 Collider::getRelativeVelocity(Collider *otherCollider) {
	Vector3 velA = mRigidBody->getLinear();
	Vector3 velB = otherCollider->getRigidBody()->getLinear();

	return velA.sub(velB);
}

// ---------------------------------------------------------------------------

bool Collider::isSimulate() {
	return mRigidBody->isSimulate();
}

void Collider::markPenetratedBy(Collider* otherCollider) {
	mIsPenetrated = true;

	Vector3 force = getCenter().sub(otherCollider->getCenter()).nor();

	mRigidBody->addAntiPenetrationForce(-force);
	otherCollider->getRigidBody()->addAntiPenetrationForce(force);
}

void Collider::unmarkPenetrated() {
	mIsPenetrated = false;

	mRigidBody->resetAntiPenetrationForce();
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testCollider(Collider *otherCollider) {

	ColliderStatus result = ColliderStatus::STATUS_NONE;

	if(mColliderShape == ColliderShape::RECTANGLE && otherCollider->getShape() == ColliderShape::RECTANGLE){
		result = testRectangleRectangle(otherCollider);
	} else if(mColliderShape == ColliderShape::RECTANGLE && otherCollider->getShape() == ColliderShape::SPHERE){
		result = testRectangleSphere(otherCollider);
	} else if(mColliderShape == ColliderShape::SPHERE && otherCollider->getShape() == ColliderShape::RECTANGLE){
		//result = otherCollider->testRectangleSphere(this);
	} else if(mColliderShape == ColliderShape::SPHERE && otherCollider->getShape() == ColliderShape::SPHERE){
		result = testSphereSphere(otherCollider);
	}

	if (result != ColliderStatus::STATUS_NONE && !(mIsSolid && otherCollider->isSolid())) {

		// SEND DIRECTLY TO PENETRATION
		result = ColliderStatus::STATUS_PENETRATION;
	}


	if (result == ColliderStatus::STATUS_PENETRATION) {

		if (mIsSolid && otherCollider->isSolid()) {

		} else {
			result = ColliderStatus::STATUS_COLLISION;
		}
	}

	return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testSphereSphere(Collider *otherCollider) {
	ColliderStatus result = ColliderStatus::STATUS_NONE;

	bool testDepthEpsilon =
			MathUtils::testSphereSphere(getCenter(), otherCollider->getCenter(), getRadius(), otherCollider->getRadius(), msDepthEpsilon);

	if (testDepthEpsilon) {
		result = ColliderStatus::STATUS_COLLISION;

		bool testZeroDepthEpsilon =
				MathUtils::testSphereSphere(getCenter(), otherCollider->getCenter(), getRadius(), otherCollider->getRadius(), 0);

		if (testZeroDepthEpsilon) {
			result = ColliderStatus::STATUS_PENETRATION;
		}
	}

	mLastContact->contactPoint = MathUtils::midPoint(getCenter(), otherCollider->getCenter());
	mLastContact->colliderA = this;
	mLastContact->colliderB = otherCollider;
	mLastContact->normal = (getCenter() - otherCollider->getCenter()).nor();
	mLastContact->relativeVelocity = otherCollider->getRigidBody()->getLinear() - getRigidBody()->getLinear();
	//RenderEngine::getInstance()->drawLine(getCenter(), otherCollider->getCenter(), 2.0f, true);

	return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testRectangleSphere(Collider *otherCollider) {

	// ASSUME OTHER COLLIDER AS SPHERE

	ColliderStatus result = ColliderStatus::STATUS_NONE;

	auto vertices = getBoundingBox(true);

	Vector2 intersectionResult1, intersectionResult2;
	Vector2 edgeStart, edgeEnd;

	FOR_ARRAY_COND(i, vertices, result == ColliderStatus::STATUS_NONE) {

		edgeStart = vertices->get(i);
		edgeEnd = vertices->get(i == 3 ? 0 : i + 1);

		Vector3 otherCentre = otherCollider->getCenter();
		f32 otherRadius = otherCollider->getRadius();

		bool testDepthEpsilon =
				MathUtils::testLineSphereSimple(
						edgeStart, edgeEnd,
						otherCentre, otherRadius, msDepthEpsilon);

		if (testDepthEpsilon) {

			// Get the intersection points
			MathUtils::testLineSphere(
				edgeStart, edgeEnd,
				otherCentre, otherRadius, msDepthEpsilon,
				intersectionResult1, intersectionResult2);

			bool testZeroDepthEpsilonEpsilon =
					MathUtils::testLineSphereSimple(
						edgeStart, edgeEnd,
						otherCentre, otherRadius, 0);

			if(testZeroDepthEpsilonEpsilon) {
				// Get the intersection points
				MathUtils::testLineSphere(
					edgeStart, edgeEnd,
					otherCentre, otherRadius, 0,
					intersectionResult1, intersectionResult2);
			}

			result = testZeroDepthEpsilonEpsilon ? ColliderStatus::STATUS_PENETRATION : ColliderStatus::STATUS_COLLISION;
		}
	}

	mLastContact->contactPoint = MathUtils::midPoint(Vector3(intersectionResult1), Vector3(intersectionResult2));
	mLastContact->colliderA = this;
	mLastContact->colliderB = otherCollider;
	mLastContact->normal = (mLastContact->contactPoint - otherCollider->getCenter()).nor();
	mLastContact->relativeVelocity = otherCollider->getRigidBody()->getLinear() - getRigidBody()->getLinear();

//	if(result > ColliderStatus::STATUS_NONE){
//		RenderEngine::getInstance()->drawLine(mLastContact->contactPoint, mLastContact->contactPoint + (mLastContact->normal * 100), 2.0f, true);
//		RenderEngine::getInstance()->drawLine(edgeStart, edgeEnd, 2.0f, true);
//	}

//	mLastContact->

	return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testRectangleRectangle(Collider *otherCollider) {


	ColliderStatus result = ColliderStatus::STATUS_NONE;

	GameObject* gameObject = getGameObject();

	Vector3 center = getCenter();
	Vector3 otherCenter = otherCollider->getCenter();

	Vector3 relativeVelocity = getRelativeVelocity(otherCollider);
	relativeVelocity.nor();

	Vector3 normal = Vector3(center).sub(otherCenter).nor();
	f32 vrn = relativeVelocity.dot(normal);

	// if(vrn < 0){

	Array<Vector2>* vertices = getBoundingBox();

	Array<Vector2>* otherVertices = otherCollider->getBoundingBox();

	// TEST Middle Vertex vs Edge
	u32 detectedVertexIndex = 0;
	FOR_ARRAY_COND(i, vertices, result == ColliderStatus::STATUS_NONE) {

		Vector2 midPoint = MathUtils::midPoint(vertices->get(i), vertices->get(i == 3 ? 0 : i + 1));

		ColliderStatus pointStatus = otherCollider->testPoint(midPoint);

		if (pointStatus > result) {
			result = pointStatus;
			detectedVertexIndex = i;
		}
	}

	if (result == ColliderStatus::STATUS_NONE || result == ColliderStatus::STATUS_COLLISION) {

		ColliderStatus result2 = ColliderStatus::STATUS_NONE;

		// TEST Vertex vs Edge
		// u32 detectedVertexIndex = 0;
		FOR_ARRAY_COND(i, vertices, result2 == ColliderStatus::STATUS_NONE)
		{
			ColliderStatus pointStatus = otherCollider->testPoint(vertices->get(i));

			if (pointStatus > result2) {
				result2 = pointStatus;
				// detectedVertexIndex = i;
			}
		}

		if (result2 > result) {
			result = result2;
		}
	}

	return result;
}

// ---------------------------------------------------------------------------

ColliderStatus Collider::testPoint(Vector2 point) {

	ColliderStatus result = ColliderStatus::STATUS_NONE;

	bool testDepthEpsilon = MathUtils::testRectanglePoint(mBoxVertices->get(0), mWidth, mHeight, point, msDepthEpsilon);

	if (testDepthEpsilon) {
		result = ColliderStatus::STATUS_COLLISION;

		bool testZeroDepthEpsilon = MathUtils::testRectanglePoint(mBoxVertices->get(0), mWidth, mHeight, point, 0.0f);

		if (testZeroDepthEpsilon) {
			result = ColliderStatus::STATUS_PENETRATION;
		}
	}

	return result;
};

// ---------------------------------------------------------------------------

bool Collider::checkCollisionRadius(Collider *otherCollider) const {
	return MathUtils::testSphereSphere(getCenter(), otherCollider->getCenter(), getRadius(), otherCollider->getRadius(), 0);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
