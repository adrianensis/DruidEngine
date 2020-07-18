#ifndef DE_COLLIDER_H
#define DE_COLLIDER_H

#include "Component.hpp"
#include "Vector2.hpp"

namespace DE {

template<class T> class Array;
class RigidBody;
class ContactsManager;

enum class ColliderStatus {
	STATUS_NONE, STATUS_COLLISION, STATUS_PENETRATION
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

public:

	DE_CLASS(Collider, Component);

	void init() override;

	void setSize(f32 width, f32 height);
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

	ColliderStatus getStatus() const {
		return mStatus;
	};

	void setStatus(ColliderStatus newStatus) {
		mStatus = newStatus;
	};


	bool isSolid() const {
		return mIsSolid;
	};

	void setIsSolid(bool isSolid) {
		mIsSolid = isSolid;
	};


	void markPenetrated() {
		mIsPenetrated = true;
	}
	void unmarkPenetrated() {
		mIsPenetrated = false;
	}
	bool isPenetrated() const {
		return mIsPenetrated;
	}

	bool isSimulate();

	bool checkCollisionRadius(Collider *otherCollider) const;

	ColliderStatus generateContacts(Array<Vector2> *candidateVertices, Collider *otherCollider/*, contactManager*/);
	ColliderStatus testVertexVertex(Array<Vector2> *candidateVertices, Collider *otherCollider/*, contactManager*/);
	ColliderStatus testVertexEdge(Array<Vector2> *candidateVertices, Collider *otherCollider/*, contactManager*/);

	ColliderStatus testRectangleRectangle(Collider *otherCollider);

	ColliderStatus testPoint(Vector2 point);
};
} /* namespace DE */
#endif /* DE_COLLIDER_H */
