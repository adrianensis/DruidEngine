#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Collider;
enum class ColliderStatus
;
template<class T> class List;
class Iterator;
template<class T> class Array;
template<class K, class V> class HashMap;
class GameObject;

class QuadTree : public ObjectBase {
private:

	class Node : public ObjectBase {
	private:
		void internalRemoveColliderFromList(const Iterator *it);
		void internalFreeCollider(Collider *collider);

	public:
		 Vector2 mLeftTop;
		 f32 mWidth;
		 f32 mHeight;
		 f32 mMinWidth;
		 f32 mMinHeight;

		 f32 mRadius;

		 QuadTree* mTree;

		 f32 mHalfWidth;
		 f32 mHalfHeight;
		 bool mIsDivisible;

		 List<Collider*>* mColliders;
		 List<Collider*>* mExitingColliders;
		 Array<Node*>* mChildren;
		 Array<Vector2>* mLeftTopChildrenArray;

		 u32 mChildrenCount;

		 u32 mDynamicCollidersCount;
		 u32 mStaticCollidersCount;

		GENERATE_METADATA(Node);

	Node();
	virtual ~Node() override;

		void init(const Vector2 &leftTop, f32 width, f32 height, f32 minWidth, f32 minHeight, QuadTree *tree);
		Node* createChildNode(u32 index);
		void addCollider(Collider *collider);
		void update();
		void updateChildren();
		void checkExit(Collider *collider) const;
		void manageExits(List<Collider*> *exitingColliders);

		bool isLeaf() const;
		u32 getCollidersCount() const;
		bool childNodeTestPartialCollider(u32 index, Collider *collider) const;

		void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

	};

	 Node* mRoot;

	 f32 mWidth;
	 f32 mHeight;

	 ColliderStatus mStatus;

public:

	GENERATE_METADATA(QuadTree);

	QuadTree();
	virtual ~QuadTree() override;;

	GET_SET(Status);

	void init(f32 size);

	void addCollider(Collider *collider);
	void update();

	void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

};
}

