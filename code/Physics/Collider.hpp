#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector2.hpp"

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

class Collider : public Component {
private:

	 Array<Vector2>* mBoxVertices;

	 f32 mHalfWidth;
	 f32 mHalfHeight;
	 ColliderStatus mStatus;
	
	static f32 msDepthEpsilon;

	 f32 mRadius;
	 f32 mHeight;
	 f32 mWidth;
	 Vector3 mPositionOffset;
	 RigidBody* mRigidBody;
	 f32 mCollisionLayer;
	 ColliderShape mShape;
	 bool mIsSolid;
	 Contact* mLastContact;
	 bool mIsPenetrated;
	 bool mHasSizeChanged;

public:

	GENERATE_METADATA(Collider);

	Collider();
	virtual ~Collider() override;;

	GET(Radius);
	GET(Height);
	GET(Width);
	GET_SET(PositionOffset);
	GET_SET(RigidBody);
	GET_SET(CollisionLayer);
	GET_SET(Shape);
	GET_SET(IsSolid);
	GET(LastContact);
	GET(IsPenetrated);
	GET(HasSizeChanged);

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

	virtual void save(ConfigMap* configMap, StringRef objectName) override;
	virtual void load(ConfigMap* configMap, StringRef objectName) override;
};
}

