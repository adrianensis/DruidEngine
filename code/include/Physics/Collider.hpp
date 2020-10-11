#ifndef DE_COLLIDER_H
#define DE_COLLIDER_H

#include "Component.hpp"
#include "Vector2.hpp"

namespace DE {

template<class T> class Array;
class RigidBody;
class ContactsManager;
class Contact;

enum class ColliderStatus {
	STATUS_NONE, STATUS_COLLISION, STATUS_PENETRATION
};

enum class ColliderShape {
	RECTANGLE, SPHERE
};

class Collider: public Component {
private:

	Array<Vector2>* mBoxVertices;

	f32 mWidth;
	f32 mHalfWidth;
	f32 mHeight;
	f32 mHalfHeight;
	f32 mRadius;
	bool mIsSolid;
	Vector3 mPositionOffset;

	static f32 msDepthEpsilon;

	RigidBody* mRigidBody;

	ColliderStatus mStatus;

	bool mIsPenetrated;

	u32 mCollisionLayer;

	ColliderShape mColliderShape;

	Contact* mLastContact;

	bool mHasSizeChanged;

public:

	DE_CLASS(Collider, Component);

	void init() override;

	void setSize(f32 width, f32 height);
	bool hasSizeChanged() const { return mHasSizeChanged; }
	void resetHasSizeChaged() { mHasSizeChanged = false; }
	f32 getHeight() const { return mHeight; }
	f32 getWidth() const { return mWidth; }
	Array<Vector2>* getBoundingBox(bool forceCalculateBoundingBox = false);
	Vector3 getRelativeVelocity(Collider *otherCollider);
	f32 getRadius() const {
		return mRadius;
	};

	const Vector3& getPositionOffset() const {
		return mPositionOffset;
	};

	void setPositionOffset(Vector3 newPositionOffset) {
		mPositionOffset = newPositionOffset;
	};

	Vector3 getCenter() const;

	RigidBody* getRigidBody() const {
		return mRigidBody;
	};

	void setRigidBody(RigidBody *newRigidBody) {
		mRigidBody = newRigidBody;
	};

	u32 getCollisionLayer() const {
		return mCollisionLayer;
	};

	void setCollisionLayer(u32 collisionLayer) {
		mCollisionLayer = collisionLayer;
	};

	bool isSolid() const {
		return mIsSolid;
	};

	void setIsSolid(bool isSolid) {
		mIsSolid = isSolid;
	};

	ColliderShape getShape() const {
		return mColliderShape;
	};

	void setShape(ColliderShape colliderShape) {
		mColliderShape = colliderShape;
	};

	Contact* getLastContact() const {
		return mLastContact;
	}

	void markPenetratedBy(Collider* otherCollider);
	void unmarkPenetrated();

	bool isPenetrated() const {
		return mIsPenetrated;
	}

	bool isSimulate();

	bool checkCollisionRadius(Collider *otherCollider) const;

	ColliderStatus testCollider(Collider *otherCollider);
	ColliderStatus testRectangleRectangle(Collider *otherCollider);
	ColliderStatus testRectangleSphere(Collider *otherCollider);
	ColliderStatus testSphereSphere(Collider *otherCollider);

	ColliderStatus testPoint(Vector2 point);
};
} /* namespace DE */
#endif /* DE_COLLIDER_H */
