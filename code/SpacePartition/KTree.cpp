
#include "SpacePartition/KTree.hpp"
#include "Graphics/Graphics.hpp"
#include "Scenes/Scenes.hpp"

void KTree::KTreeNode::init(const Rectangle& rectangle, const Vector3& minSize, KTree& tree)
{
	mRectangle = rectangle;

	mRadius = sqrt((mRectangle.getSize().x * mRectangle.getSize().x) + (mRectangle.getSize().y * mRectangle.getSize().y)) / 2.0f;
	// mTree = tree;

	mHalfSize = mRectangle.getSize() / 2.0f;
	mIsDivisible = (mHalfSize.x >= minSize.x) && (mHalfSize.y >= minSize.y);

	// mColliders = Memory::allocate<List<Collider*>>();
	// mColliders->init();

	// mExitingColliders = Memory::allocate<List<Collider*>>();
	// mExitingColliders->init();

	//mChildren = Memory::allocate<Array<Node*>>();

	if(mIsDivisible)
	{
		mChildren.reserve(4);
		
		mChildren.emplace_back(KTreeNode()).
		init(Rectangle(Vector2(mRectangle.getLeftTop().x, mRectangle.getLeftTop().y), mHalfSize), minSize, tree);
		
		mChildren.emplace_back(KTreeNode()).
		init(Rectangle(Vector2(mRectangle.getLeftTop().x, mRectangle.getLeftTop().y - mHalfSize.y), mHalfSize), minSize, tree);
		
		mChildren.emplace_back(KTreeNode()).
		init(Rectangle(Vector2(mRectangle.getLeftTop().x + mHalfSize.x, mRectangle.getLeftTop().y - mHalfSize.y), mHalfSize), minSize, tree);
		
		mChildren.emplace_back(KTreeNode()).
		init(Rectangle(Vector2(mRectangle.getLeftTop().x + mHalfSize.x, mRectangle.getLeftTop().y), mHalfSize), minSize, tree);
	}

	// mLeftTopChildrenArray = Memory::allocate<Array<Vector2>>();
	// mLeftTopChildrenArray->init(4);

	// // This array testPartialCollider the LeftTop Vertices of the 4 children.
	// mLeftTopChildrenArray->set(0, );
	// mLeftTopChildrenArray->set(1, );
	// mLeftTopChildrenArray->set(2, );
	// mLeftTopChildrenArray->set(3, );

}

//----------------------------------------------------------------------

bool QuadTree::KTreeNode::childNodeTestPartial(u32 index, IKTreeElement& element) const
{
	bool test = Geometry::testRectangleSphere(mChildren[index].mRectangle.getLeftTop(), mHalfSize.x, mHalfSize.y, mHalfSize.z,
				element.getKTreeElementCenter(), element.getKTreeElementRadius(), 0);

	return test;
};

//----------------------------------------------------------------------

void QuadTree::KTreeNode::addKTreeElement(IKTreeElement& element)
{
	if (mIsDivisible)
	{
		// For each "possible" child node
		FOR_ARRAY(i, mChildren)
		{
			KTreeNode& node = mChildren[i];
			bool isPartiallyInChildren = childNodeTestPartial(i, element);
			if (isPartiallyInChildren)
			{
				node.addKTreeElement(element);
			}
		}
	}
	else
	{
		// Add Element to node
		mKTreeElements.emplace_back(&element);
	}
}

//----------------------------------------------------------------------

// void QuadTree::Node::internalRemoveColliderFromList(const Iterator *it)
// {
// 	auto castedIt = it->cast<Collider*>();
// 	mColliders->remove(*castedIt);

// }

// //----------------------------------------------------------------------

// void QuadTree::Node::internalFreeCollider(Collider *collider)
// {
// 	collider->setDestroyed();
// 	Memory::free<Collider>(collider);
// }

//----------------------------------------------------------------------

void KTree::KTreeNode::update(/*contactManager*/)
{
	// DEBUG DRAW
	RenderEngine::getInstance().drawRectangle(mRectangle);

	// If is leaf node.
	if (isLeaf())
	{
		ECHO("LEAF")
	}
	else
	{
		updateChildren(/*contactManager*/);
	}
};

//----------------------------------------------------------------------

void KTree::KTreeNode::updateChildren(/*contactManager*/)
{
	FOR_LIST(it, mChildren)
	{
		KTreeNode& child = *it;
		child.update(/*contactManager*/);
	}
};

//----------------------------------------------------------------------

// void QuadTree::Node::checkExit(Collider *collider) const {
// 	// CHECK if collider is out of this node.
// 	// only dynamic objects can escape from their nodes !!!

// 	if (!collider->isStatic() || collider->getHasSizeChanged())
// {

// //		bool collision = MathUtils::testRectangleSphere(mLeftTop, mWidth, mHeight,
// //						Vector2(collider->getCenter()), collider->getRadius(), 0);

// 		bool collision = MathUtils::testRectanglePoint(mLeftTop, mWidth, mHeight,
// 								Vector2(collider->getCenter()), -collider->getRadius());

// 		if(!collision || collider->getHasSizeChanged()){
// 			mExitingColliders->pushBack(collider);
// 			mTree->addCollider(collider);
// 		}
// 	}
// };

// //----------------------------------------------------------------------

// void QuadTree::Node::manageExits(List<Collider*> *exitingColliders)
// {

// 	// If any collider has left the node
// 	if (exitingColliders->getLength() > 0)
// {

// 		// FOR EACH COLLIDER
// 		FOR_LIST (itExiting, exitingColliders)
// 		{
// 			Collider* collider = itExiting.get();
// 			mColliders->remove(mColliders->find(collider));

// 			// Note: Only dynamic colliders can leave the node!
// 			if (!collider->isStatic())
// {
// 				mDynamicCollidersCount--;
// 			}
// 		}
// 	}
// };

//----------------------------------------------------------------------

bool KTree::KTreeNode::isLeaf() const
{
	return mChildren.size() == 0;
};
// u32 QuadTree::Node::getCollidersCount() const {
// 	return mColliders->getLength();
// };

// void QuadTree::Node::rayCastQuery(const Vector3 &lineStart, const Vector3 &lineEnd, List<GameObject*> *outList)
// {

// 	bool rayIntersectsNode = MathUtils::testLineSphereSimple(Vector2(lineStart), Vector2(lineEnd),
// 			Vector2(mLeftTop + Vector3(mHalfWidth, -mHalfHeight, 0)), mRadius, 0.0f);

// 	if (rayIntersectsNode)
// {
// 		if (isLeaf())
// {

// 			FOR_LIST(it, mColliders)
// {
// 				Collider* collider = it.get();

// 				GameObject* gameObject = collider->getGameObject();

// 				// TODO : line vs rectangle
// 				bool rayIntersectsCollider = MathUtils::testLineSphereSimple(Vector2(lineStart), Vector2(lineEnd),
// 						Vector2(collider->getCenter()), collider->getRadius(), 0.0f);

// 				if (rayIntersectsCollider)
// {
// 					List<GameObject*>::ListIterator it = outList->find(gameObject);

// 					// If not found then add it
// 					if (it.isNull())
// {
// 						outList->pushBack(gameObject);
// 					}
// 				}
// 			}

// 		} else {
// 			FOR_ARRAY (i, mChildren)
// {
// 				Node* child = mChildren->get(i);

// 				if (child)
// {
// 					child->rayCastQuery(lineStart, lineEnd, outList);
// 				}
// 			}
// 		}
// 	}
// }

// QuadTree::QuadTree()
// {
// 	mWidth = 0.0f;
// 	mHeight = 0.0f;
// 	//mStatus = ColliderStatus::STATUS_NONE;
// }

// QuadTree::~QuadTree()
// {
// 	//DELETE(mRoot)
// }

void KTree::init(f32 size)
{
	mSize.set(size, size, size);

	//f32 minSize = Settings::getInstance()->getF32("scene.quadTreeMinSize");

	f32 minSize = size / 8.0f;
	mRoot.init(Rectangle(Vector3(-mSize.x / 2.0f, mSize.y / 2.0f, mSize.z / 2.0f), mSize), Vector3(minSize, minSize, minSize), *this);
}

void KTree::update()
{
	mRoot.update();
}

// void QuadTree::addCollider(Collider *collider)
// {
// 	collider->resetHasSizeChaged();
// 	mRoot->addCollider(collider);
// }

// void QuadTree::rayCastQuery(const Vector3 &lineStart, const Vector3 &lineEnd, List<GameObject*> *outList)
// {
// 	mRoot->rayCastQuery(lineStart, lineEnd, outList);
// }