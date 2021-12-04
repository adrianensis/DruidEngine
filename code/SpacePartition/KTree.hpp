#pragma once

#include "Core/Core.hpp"

class IKTreeElement: public ObjectBase
{
	GENERATE_METADATA(IKTreeElement)

	PUB virtual Vector3 getKTreeElementCenter() const = 0;
	PUB virtual f32 getKTreeElementRadius() const = 0;
};

// Parent class for QuadTree and OcTree
class KTree: public ObjectBase
{
	GENERATE_METADATA(KTree)

    PUB ~KTree() override {};

    PRO class KTreeNode: public ObjectBase
	{
		GENERATE_METADATA(KTree::KTreeNode)

		PRI std::vector<IKTreeElement*> mKTreeElements;

        PUB Rectangle mRectangle;
        PUB f32 mRadius = 0;
        PUB Vector3 mHalfSize;
		PUB bool mIsDivisible = false;

		PUB std::vector<KTreeNode> mChildren;


        PUB void init(const Rectangle& rectangle, const Vector3& minSize, KTree& tree);
		PUB void addKTreeElement(IKTreeElement& element);
		PUB void update();
		PRI void updateChildren();
		PRI bool childNodeTestPartial(u32 index, IKTreeElement& element) const;
		//void checkExit(Collider *collider) const;
		//void manageExits(List<Collider*> *exitingColliders);

		PUB bool isLeaf() const;
    };

    PRI KTreeNode mRoot;

	PUB Vector3 mSize;

	//DE_M_GET_SET(Status, ColliderStatus)


	PUB void init(f32 size);

	//PUB void addCollider(Collider *collider);
	PUB void update();
};

class QuadTree: public KTree
{
	GENERATE_METADATA(QuadTree)

	PRI class QuadTreeNode: public KTreeNode
	{
		GENERATE_METADATA(QuadTree::QuadTreeNode)
	//private:
		//void internalRemoveColliderFromList(const Iterator *it);
		//void internalFreeCollider(Collider *collider);

		//DE_M(Tree, QuadTree*)

		//DE_M(Colliders, List<Collider*>*)
		//DE_M(ExitingColliders, List<Collider*>*)
		//DE_M(Children, Array<Node*>*)
		//DE_M(LeftTopChildrenArray, Array<Vector2>*)

		//DE_M(DynamicCollidersCount, u32)
		//DE_M(StaticCollidersCount, u32)

		//DE_CLASS(Node)

		//PUB u32 getCollidersCount() const;

		//PUB void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);
	};

	//PUB void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

};