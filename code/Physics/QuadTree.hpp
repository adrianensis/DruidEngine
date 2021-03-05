#pragma once

#include "Core/DE_Class.hpp"
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

class QuadTree: public DE_Class {
private:

	class Node: public DE_Class {
	private:
		void internalRemoveColliderFromList(const Iterator *it);
		void internalFreeCollider(Collider *collider);

	public:
		DE_M(LeftTop, Vector2)
		DE_M(Width, f32)
		DE_M(Height, f32)
		DE_M(MinWidth, f32)
		DE_M(MinHeight, f32)

		DE_M(Radius, f32)

		DE_M(Tree, QuadTree*)

		DE_M(HalfWidth, f32)
		DE_M(HalfHeight, f32)
		DE_M(IsDivisible, bool)

		DE_M(Colliders, List<Collider*>*)
		DE_M(ExitingColliders, List<Collider*>*)
		DE_M(Children, Array<Node*>*)
		DE_M(LeftTopChildrenArray, Array<Vector2>*)

		DE_M(ChildrenCount, u32)

		DE_M(DynamicCollidersCount, u32)
		DE_M(StaticCollidersCount, u32)

		DE_CLASS_BODY(Node)

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

	DE_M(Root, Node*)

	DE_M(Width, f32)
	DE_M(Height, f32)

	DE_M_GET_SET(Status, ColliderStatus)

public:

	DE_CLASS_BODY(QuadTree)

	void init(f32 size);

	void addCollider(Collider *collider);
	void update();

	void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

};
}

