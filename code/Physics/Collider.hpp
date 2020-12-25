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
	ColliderShape mShape;
	Contact* mLastContact;
	bool mHasSizeChanged;

public:

	DE_CLASS(Collider, Component)


	DE_GET(Radius)
	DE_GET(Height)
	DE_GET(Width)
	DE_GET_SET(PositionOffset)
	DE_GET_SET(RigidBody)
	DE_GET_SET(CollisionLayer)
	DE_GET_SET(Shape)
	DE_GET_SET(IsSolid)
	DE_GET(LastContact)
	DE_GET(IsPenetrated)
	DE_GET(HasSizeChanged)

	void init() override;

	Vector3 getCenter() const;
	void setSize(f32 width, f32 height);
	void resetHasSizeChaged() { mHasSizeChanged = false; }
	Array<Vector2>* getBoundingBox(bool forceCalculateBoundingBox = false);
	Vector3 getRelativeVelocity(Collider *otherCollider);

	void markPenetratedBy(Collider* otherCollider);
	void unmarkPenetrated();
	bool isSimulate();

	bool checkCollisionRadius(Collider *otherCollider) const;

	ColliderStatus testCollider(Collider *otherCollider);
	ColliderStatus testRectangleRectangle(Collider *otherCollider);
	ColliderStatus testRectangleSphere(Collider *otherCollider);
	ColliderStatus testSphereSphere(Collider *otherCollider);

	ColliderStatus testPoint(Vector2 point);

	void render();
};
} /* namespace DE */
#endif /* DE_COLLIDER_H */
