#include "Physics/Collider.hpp"
#include "Containers/Array.hpp"
#include "Containers/List.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Scene/GameObject.hpp"
#include "Physics/RigidBody.hpp"
#include "Scene/Transform.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Memory/Memory.hpp"
#include "Log/Log.hpp"
#include "Maths/MathUtils.hpp"
#include "ContactsManager.hpp"
#include "Graphics/Renderer.hpp"

namespace DE {

f32 Collider::msDepthEpsilon = 10.0f;

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
	mShape = ColliderShape::RECTANGLE;
	mLastContact = nullptr;
	mHasSizeChanged = false;
}

Collider::~Collider() {
	if (mBoxVertices) {
		Memory::free(mBoxVertices);
	}

	Memory::free(mLastContact); // TODO: remove contact here or in contact manager?
}

void Collider::init() {
	// TRACE();

	mBoxVertices = Memory::allocate<Array<Vector2>>();
	mBoxVertices->init(4);

	mBoxVertices->set(0, Vector2(0, 0)); // LEFT TOP VERTEX
	mBoxVertices->set(1, Vector2(0, 0)); // LEFT BOTTOM
	mBoxVertices->set(2, Vector2(0, 0)); // RIGHT BOTTOM
	mBoxVertices->set(3, Vector2(0, 0)); // RIGHT TOP

	mRigidBody = getGameObject()->getFirstComponent<RigidBody>();

	mPositionOffset.set(0, 0, 0);

	mCollisionLayer = 0;

	mLastContact = Memory::allocate<Contact>();
	mLastContact->init();
}

void Collider::setSize(f32 width, f32 height) {
	mWidth = width;
	mHalfWidth = width / 2.0f;
	mHeight = height;
	mHalfHeight = height / 2.0f;

	mRadius = sqrt((mWidth * mWidth) + (mHeight * mHeight)) / 2.0f;

	getBoundingBox(true);

	mHasSizeChanged = true;
}

Array<Vector2>* Collider::getBoundingBox(bool forceCalculateBoundingBox/* = false*/) {

	Transform* t = getGameObject()->getTransform();

	if (t->isDirtyTranslation() || forceCalculateBoundingBox) { // TODO : add condition -> !isStatic() ?
		Vector3 center = getCenter();

		mBoxVertices->set(0, Vector2(center.x - mHalfWidth, center.y + mHalfHeight)); // LEFT TOP VERTEX
		const Vector2& LeftTop = mBoxVertices->get(0);
		mBoxVertices->set(1, Vector2(LeftTop.x, LeftTop.y - mHeight)); // LEFT BOTTOM
		mBoxVertices->set(2, Vector2(LeftTop.x + mWidth, LeftTop.y - mHeight)); // RIGHT BOTTOM
		mBoxVertices->set(3, Vector2(LeftTop.x + mWidth, LeftTop.y)); // RIGHT TOP
	}

	return mBoxVertices;
}

Vector3 Collider::getCenter() const {
	return getGameObject()->getTransform()->getWorldPosition() + mPositionOffset;
}

Vector3 Collider::getRelativeVelocity(Collider *otherCollider) {
	Vector3 velA = mRigidBody->getLinear();
	Vector3 velB = otherCollider->getRigidBody()->getLinear();

	return velA.sub(velB);
}

bool Collider::isSimulate() {
	return mRigidBody->getSimulate();
}

void Collider::markPenetratedBy(Collider* otherCollider) {
	mIsPenetrated = true;

	Vector3 force = getCenter().sub(otherCollider->getCenter()).nor();

	mRigidBody->addAntiPenetrationForce(-force);
	//otherCollider->getRigidBody()->addAntiPenetrationForce(force);
}

void Collider::unmarkPenetrated() {
	mIsPenetrated = false;

	mRigidBody->resetAntiPenetrationForce();
}

ColliderStatus Collider::testCollider(Collider *otherCollider) {

	ColliderStatus result = ColliderStatus::STATUS_NONE;

	if(mShape == ColliderShape::RECTANGLE && otherCollider->getShape() == ColliderShape::RECTANGLE){
		result = testRectangleRectangle(otherCollider);
	} else if(mShape == ColliderShape::RECTANGLE && otherCollider->getShape() == ColliderShape::SPHERE){
		result = testRectangleSphere(otherCollider);
	} else if(mShape == ColliderShape::SPHERE && otherCollider->getShape() == ColliderShape::RECTANGLE){
		//result = otherCollider->testRectangleSphere(this);
	} else if(mShape == ColliderShape::SPHERE && otherCollider->getShape() == ColliderShape::SPHERE){
		result = testSphereSphere(otherCollider);
	}

	if (result != ColliderStatus::STATUS_NONE && !(mIsSolid && otherCollider->getIsSolid())) {

		// SEND DIRECTLY TO PENETRATION
		result = ColliderStatus::STATUS_PENETRATION;
	}

	if (result == ColliderStatus::STATUS_PENETRATION) {

		if (mIsSolid && otherCollider->getIsSolid()) {

		} else {
			result = ColliderStatus::STATUS_COLLISION;
		}
	}

	return result;
}

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

	return result;
}

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

	return result;
}

ColliderStatus Collider::testRectangleRectangle(Collider *otherCollider) {

	ColliderStatus result = ColliderStatus::STATUS_NONE;

	GameObject* gameObject = getGameObject();

	Vector3 center = getCenter();
	Vector3 otherCenter = otherCollider->getCenter();

	Vector3 relativeVelocity = getRelativeVelocity(otherCollider);
	relativeVelocity.nor();

	Vector3 normal = Vector3(center).sub(otherCenter).nor();
	f32 vrn = relativeVelocity.dot(normal);

	Array<Vector2>* vertices = getBoundingBox();
	Array<Vector2>* otherVertices = otherCollider->getBoundingBox();

	Vector3 contactPoint(0,0,0);

	// TEST Middle Vertex vs Edge
	FOR_ARRAY_COND(i, vertices, result == ColliderStatus::STATUS_NONE) {

		Vector2 midPoint = MathUtils::midPoint(vertices->get(i), vertices->get(i == 3 ? 0 : i + 1));

		ColliderStatus pointStatus = otherCollider->testPoint(midPoint);

		if (pointStatus > result) {
			result = pointStatus;
			contactPoint = midPoint;
		}
	}

	if (result < ColliderStatus::STATUS_PENETRATION) {

		ColliderStatus result2 = ColliderStatus::STATUS_NONE;

		// TEST Vertex
		FOR_ARRAY_COND(i, vertices, result2 == ColliderStatus::STATUS_NONE)
		{
			ColliderStatus pointStatus = otherCollider->testPoint(vertices->get(i));

			if (pointStatus > result2) {
				result2 = pointStatus;
				contactPoint = vertices->get(i);
			}
		}

		if (result2 > result) {
			result = result2;
		}
	}

	mLastContact->contactPoint = contactPoint;
	mLastContact->colliderA = this;
	mLastContact->colliderB = otherCollider;
	mLastContact->normal = (mLastContact->contactPoint - otherCollider->getCenter()).nor();
	mLastContact->relativeVelocity = otherCollider->getRigidBody()->getLinear() - getRigidBody()->getLinear();

	return result;
}

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

bool Collider::checkCollisionRadius(Collider *otherCollider) const {
	return MathUtils::testSphereSphere(getCenter(), otherCollider->getCenter(), getRadius(), otherCollider->getRadius(), 0);
}

void Collider::render() {

	bool isAffectedByProjection = getGameObject()->getTransform()->getAffectedByProjection();

	if (isAffectedByProjection) {
		Collider* collider = this;
		f32 lineSize = 1;

		// TODO : make custom debug shapes for rectangle and circle.
		if(collider->getShape() == ColliderShape::RECTANGLE) {
			Array<Vector2>* box = collider->getBoundingBox(true);

			RenderEngine::getInstance()->drawLine(Vector3(box->get(0)), Vector3(box->get(1)), lineSize, isAffectedByProjection);
			RenderEngine::getInstance()->drawLine(Vector3(box->get(1)), Vector3(box->get(2)), lineSize, isAffectedByProjection);
			RenderEngine::getInstance()->drawLine(Vector3(box->get(2)), Vector3(box->get(3)), lineSize, isAffectedByProjection);
			RenderEngine::getInstance()->drawLine(Vector3(box->get(3)), Vector3(box->get(0)), lineSize, isAffectedByProjection);
		}

		if(collider->getShape() == ColliderShape::SPHERE) {
			Vector3 center = collider->getCenter();
			Vector2 radiusVector(0,collider->getRadius());
			Vector3 firstVertex = Vector3(center) + radiusVector;
			Vector3 lastVertex = firstVertex;
			Matrix4 rotation;
			f32 edgesCount = 10;

			FOR_RANGE(i, 0, edgesCount){

				rotation.rotation(Vector3(0,0, i * (360.0f/edgesCount)));

				Vector3 rotatedVector = rotation.mulVector(radiusVector);
				rotatedVector += Vector3(center);

				RenderEngine::getInstance()->drawLine( lastVertex, rotatedVector, lineSize, isAffectedByProjection);

				lastVertex = rotatedVector;
			}

			RenderEngine::getInstance()->drawLine( lastVertex, firstVertex, lineSize, isAffectedByProjection);
		}
	}
}

void Collider::save(ConfigMap* configMap, StringRef objectName) {
	configMap->setF32(objectName + ".collider.width", getWidth());
	configMap->setF32(objectName + ".collider.height", getHeight());
}

void Collider::load(ConfigMap* configMap, StringRef objectName) {
	setSize(configMap->getF32(objectName + ".collider.width"), configMap->getF32(objectName + ".collider.height"));
}

}
