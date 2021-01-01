#ifndef DE_COLLIDER_H
#define DE_COLLIDER_H

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

class Collider: public Component {
private:

	DE_M(BoxVertices, Array<Vector2>*)

	DE_M(HalfWidth, f32)
	DE_M(HalfHeight, f32)
	DE_M(Status, ColliderStatus)
	
	static f32 msDepthEpsilon;

	DE_M_GET(Radius, f32)
	DE_M_GET(Height, f32)
	DE_M_GET(Width, f32)
	DE_M_GET_SET(PositionOffset, Vector3)
	DE_M_GET_SET(RigidBody, RigidBody*)
	DE_M_GET_SET(CollisionLayer, f32)
	DE_M_GET_SET(Shape, ColliderShape)
	DE_M_GET_SET(IsSolid, bool)
	DE_M_GET(LastContact, Contact*)
	DE_M_GET(IsPenetrated, bool)
	DE_M_GET(HasSizeChanged, bool)

public:

	DE_CLASS(Collider)

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
